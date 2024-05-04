//
// Created by jm on 5/4/24.
//

#ifndef JM_NGINX_MYSIGNAL_H
#define JM_NGINX_MYSIGNAL_H

#include <string>
#include <csignal>

typedef struct
{
    int sig_no;
    std::string sigName;
    void (*handler) (int sig_no, siginfo_t *siginfo, void *ucontext);
} ngx_signal_t;

void ngx_signal_handler(int signo, siginfo_t *siginfo, void *ucontext);

int ngx_init_signals();

#endif //JM_NGINX_MYSIGNAL_H
