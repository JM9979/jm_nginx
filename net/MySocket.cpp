//
// Created by jm on 5/11/24.
//

#include "MySocket.h"
#include "log.h"
#include "ngx_conf.h"

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <string>

bool set_nonblocking(int fd) {
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
    read_config();
}

MySocket::~MySocket() {
    close_listening_port();
    for(auto &iter : m_listen_port_list) {
        delete (iter);
    }
    m_listen_port_list.clear();
}

bool MySocket::socket_init() {
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