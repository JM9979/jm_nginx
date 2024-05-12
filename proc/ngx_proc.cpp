//
// Created by jm on 4/29/24.
//

#include "ngx_proc.h"
#include "log.h"
#include "ngx_conf.h"
#include "ngx_func.h"
#include "MySocket.h"

#include "csignal"

#include <iostream>

const char *WorkProcessName = "jm_nginx:work process";

extern MySocket g_socket;

[[noreturn]] void ngx_master_process_loop() {
    sigset_t set;
    sigemptyset(&set);

    sigaddset(&set, SIGCHLD);   // 僵尸进程
    sigaddset(&set, SIGALRM);   // 定时器
    sigaddset(&set, SIGIO);     // 信号驱动IO
    sigaddset(&set, SIGINT);    // 中断信号
    sigaddset(&set, SIGHUP);    // 连接断开
    sigaddset(&set, SIGUSR1);   // 用户自定义1
    sigaddset(&set, SIGUSR2);   // 用户自定义2
    sigaddset(&set, SIGWINCH);  // 终端窗口大小改变
    sigaddset(&set, SIGTERM);   // 终止信号
    sigaddset(&set, SIGQUIT);   // 终端退出符

    if(sigprocmask(SIG_BLOCK, &set, nullptr) == -1) {
        LOG_ERROR << "ngx_master_process_loop sigprocmask" << std::endl;
    }

    int processNums = ngx_conf::getInstance().getInt("WorkProcess", 1);
    ngx_fork_process(processNums, WorkProcessName);

    sigemptyset(&set);
    while(true) {
        sigsuspend(&set);
    }
}

void ngx_fork_process(int processNums, const char * processName) {
    for(int i = 0; i < processNums; ++i) {
        pid_t pid = fork();
        switch(pid) {
            case -1: {
                LOG_ERROR << "ngx_fork_process fork() error" << std::endl;
                return ;
            }
            case 0:{
                ngx_work_process_init(processName);
                ngx_work_process_loop();
            }
            default :{
                break;
            }
        }
    }
}

void ngx_work_process_init(const char * processName) {
    sigset_t set;
    sigemptyset(&set);
    if(sigprocmask(SIG_SETMASK, &set, nullptr) == -1) {
        LOG_ERROR << "ngx_work_process_init sigprocmask()" << std::endl;
    }
    ngx_func::ngx_setProTitle(processName);
    g_socket.socket_init();
    g_socket.epoll_init();
}

[[noreturn]] void ngx_work_process_loop() {
    while(true) {
//        LOG_INFO << "this is work process"<< std::endl;
    }
}
