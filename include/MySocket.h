//
// Created by jm on 5/11/24.
//

#ifndef JM_NGINX_MYSOCKET_H
#define JM_NGINX_MYSOCKET_H

#include <vector>
#include <functional>
#include <sys/socket.h>

typedef struct listening_t listening_s, *ptr_listening_s;
typedef struct connection_t connection_s, *ptr_connection_s;

typedef class MySocket MySocket;
typedef int (MySocket::*handler) (ptr_connection_s ptr);

const int listen_backlog = 511;

typedef struct listening_t {
    int port;
    int fd;
    ptr_connection_s connection;
} listening_s, *ptr_listening_s;

typedef struct connection_t {
    int fd;
    ptr_listening_s listening;
    struct sockaddr sock_addr;

     handler event_handler;

    ptr_connection_s next;
} connection_s, *ptr_connection_s;

class MySocket {
public:
    MySocket();
    ~MySocket();
    bool socket_init();
    int epoll_init();
    int epoll_add_event(int fd, ptr_connection_s ptr) const;
//    int epoll_process_events();
private:
    void read_config();
    bool listening_port();
    void close_listening_port();

    void connection_pool_init();
    ptr_connection_s get_connection(int fd);
    void release_connection(ptr_connection_s ptr);

    int accept_handler(ptr_connection_s ptr);
    int rw_handler(ptr_connection_s ptr);

private:
    int m_listen_port_count;

    int m_max_connections;
    int m_free_connections;

    int m_epoll_fd;

    ptr_connection_s m_connections_list;    // 连接池的首地址, 资源释放的时候需要用到
    ptr_connection_s m_free_connection_list;    // 空闲连接的链表

    std::vector<ptr_listening_s> m_listen_port_list;
};


#endif //JM_NGINX_MYSOCKET_H