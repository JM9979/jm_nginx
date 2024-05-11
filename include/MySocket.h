//
// Created by jm on 5/11/24.
//

#ifndef JM_NGINX_MYSOCKET_H
#define JM_NGINX_MYSOCKET_H

#include <vector>

const int listen_backlog = 511;

typedef struct {
    int port;
    int fd;
} listening_s, *ptr_listening_s;

class MySocket {
public:
    MySocket();
    ~MySocket();
    bool socket_init();
private:
    bool listening_port();
    void close_listening_port();
    int m_listen_port_count;
    std::vector<ptr_listening_s> m_listen_port_list;
};


#endif //JM_NGINX_MYSOCKET_H
