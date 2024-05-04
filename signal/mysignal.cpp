//
// Created by jm on 5/4/24.
//
#include <iostream>
#include <cstring>
#include "log.h"
#include "mysignal.h"

ngx_signal_t signals_map[] = {
        {SIGHUP, "SIGHUP", ngx_signal_handler},
        {SIGINT, "SIGINT", ngx_signal_handler},
        {SIGTERM, "SIGTERM", ngx_signal_handler},
        {SIGCHLD, "SIGCHLD", ngx_signal_handler},
        {SIGQUIT, "SIGQUIT", ngx_signal_handler},
        {SIGIO, "SIGIO", ngx_signal_handler},
        {SIGSYS, "SIGSYS, SIG_IGN", nullptr},

        {0, "", nullptr}
};

int ngx_init_signals() {
    struct sigaction sa;

    for(auto &iter : signals_map) {
        memset(&sa, 0, sizeof(sa));
        if(iter.handler) {
            sa.sa_flags = SA_SIGINFO;
            sa.sa_sigaction = iter.handler;
        } else {
            sa.sa_handler = SIG_IGN;
        }

        sigemptyset(&sa.sa_mask);
        if(sigaction(iter.sig_no, &sa, nullptr) == -1) {
            LOG_ERROR << iter.sig_no << " sigaction error" << std::endl;
            return -1;
        }
    }
    return 0;
}

void ngx_signal_handler(int signo, siginfo_t *siginfo, void *ucontext) {
    std::cout << signo << ": " << std::endl;
}
