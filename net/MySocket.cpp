//
// Created by jm on 5/11/24.
//

#include "MySocket.h"
#include "log.h"
#include "ngx_conf.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <string>
#include <sys/epoll.h>
#include <errno.h>

bool set_nonblocking(int fd)  {
    int old = fcntl(fd, F_GETFL);
    if(old == -1) {
        return false;
    }
    old |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, old) == -1) {
        return false;
    }
    return true;
}

MySocket::MySocket() : m_listen_port_count(1), m_max_connections(1024) {

}

MySocket::~MySocket() {
    close_listening_port();
    for(auto &iter : m_listen_port_list) {
        delete (iter);
    }
    m_listen_port_list.clear();
}

bool MySocket::socket_init() {
    read_config();
    return listening_port();
}

void MySocket::read_config() {
    m_listen_port_count = ngx_conf::getInstance().getInt("ListenPortCount", m_listen_port_count);
    m_max_connections = ngx_conf::getInstance().getInt("MaxConnections", m_max_connections);
}

bool MySocket::listening_port() {

    int socket_fd;

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_family = AF_INET;

    for(int i = 0; i < m_listen_port_count; ++i) {
        if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            LOG_ERROR << "MySocket::listening_port() socket" << std::endl;
            return false;
        }

        if(!set_nonblocking(socket_fd)) {
            LOG_ERROR << "MySocket::listening_port() set_nonblocking" << std::endl;
            close(socket_fd);
            return false;
        }

        int reuse = 1;
        if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1) {
            LOG_ERROR << "MySocket::listening_port() setsockopt" << std::endl;
            close(socket_fd);
            return false;
        }

        std::string listen_port = "ListenPort" + std::to_string(i);
        int port = ngx_conf::getInstance().getInt(listen_port.c_str());
        server_addr.sin_port = htons(port);
        if(bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
            LOG_ERROR << "MySocket::listening_port() bind" << std::endl;
            close(socket_fd);
            return false;
        }

        if(listen(socket_fd, listen_backlog) == -1) {
            LOG_ERROR << "MySocket::listening_port() listen" << std::endl;
            close(socket_fd);
            return false;
        }

        ptr_listening_s ptr = new listening_s;
        ptr->port = port;
        ptr->fd = socket_fd;
        m_listen_port_list.emplace_back(ptr);
        LOG_INFO << "监听端口成功 port: " << port << "socket_fd: " << socket_fd << std::endl;
    }
    return true;
}

void MySocket::close_listening_port() {
    for(auto &iter : m_listen_port_list) {
        close(iter->fd);
    }
}

int MySocket::epoll_init() {
    if((m_epoll_fd = epoll_create(5)) == -1) {
        LOG_ERROR << "MySocket::epoll_init() epoll_create(5)" << std::endl;
        exit(2);
    }

    connection_pool_init();

    for(auto &iter : m_listen_port_list) {
        if((iter->connection = get_connection(iter->fd)) == nullptr) {
            LOG_ERROR << "MySocket::epoll_init() get_connection" << std::endl;
            exit(2);
        }
        iter->connection->listening = iter;

        if(epoll_add_event(iter->fd, iter->connection) == -1) {
            LOG_ERROR << "MySocket::epoll_init() epoll_add_event" << std::endl;
            exit(2);
        }
        iter->connection->event_handler = &MySocket::accept_handler;
    }
    return 0;
}

int MySocket::epoll_add_event(int fd, ptr_connection_s ptr) const {
    struct epoll_event event;
    memset(&event, 0, sizeof(struct epoll_event));
    event.events = EPOLLIN | EPOLLRDHUP;
    event.data.ptr = ptr;
    if(epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
        LOG_ERROR << "epoll_add_event() epoll_ctl" << std::endl;
        return -1;
    }
    return 0;
}

void MySocket::connection_pool_init() {
    m_connections_list = new connection_s[m_max_connections];

    m_free_connections = m_max_connections;

    ptr_connection_s next = nullptr;
    for(int i = m_max_connections - 1; i >= 0; --i) {
        m_connections_list[i].next = next;
        next = &m_connections_list[i];
    }
    m_free_connection_list = next;
}

ptr_connection_s MySocket::get_connection(int fd) {
    m_free_connections--;
    ptr_connection_s tmp = m_free_connection_list;
    m_free_connection_list = tmp->next;
    tmp->fd = fd;
    tmp->next = nullptr;
    return tmp;
}

void MySocket::release_connection(ptr_connection_s ptr) {
    m_free_connections++;
    memset(ptr, 0, sizeof(connection_s));
    ptr->next = m_free_connection_list;
    m_free_connection_list = ptr;
}

int MySocket::accept_handler(ptr_connection_s ptr) {
    struct sockaddr_in client_addr;
    memset(&client_addr, 0, sizeof(struct sockaddr_in));
    socklen_t len = sizeof(struct sockaddr_in);
    int client_fd = -1;
    while(true) {
        if((client_fd = accept(ptr->fd, (struct sockaddr *)&client_addr, &len)) == -1) {
            LOG_ERROR << "accept_handler() accept" << std::endl;
            return -1;
        }

        if(client_fd == -1) {
            if(errno == EAGAIN) {
                continue;
            }
            break;
        }

        if(!set_nonblocking(client_fd)) {
            LOG_ERROR << "accept_handler() set_nonblocking" << std::endl;
            return -1;
        }

        ptr_connection_s ptr_ = get_connection(client_fd);
        if(ptr_ == nullptr) {
            LOG_ERROR << "accept_handler() get_connection" << std::endl;
            return -1;
        }
        memcpy(&(ptr_->sock_addr), &client_addr, len);
        ptr_->fd = client_fd;
        ptr_->event_handler = &MySocket::rw_handler;
        ptr_->listening = ptr->listening;

        if(epoll_add_event(client_fd, ptr_) == -1) {
            LOG_ERROR << "accept_handler() epoll_add_event()" << std::endl;
            return -1;
        }
        break;
    }
    return 0;
}

int MySocket::rw_handler(ptr_connection_s ptr) {
    return {};
}