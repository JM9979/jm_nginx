//
// Created by jm on 4/29/24.
//

#ifndef JM_NGINX_NGX_PROC_H
#define JM_NGINX_NGX_PROC_H

// ngx_master_process_loop 主进程loop
// ngx_swap_process 创建子进程

// ngx_work_process_init 子进程初始化，清空信号集
// ngx_work_process_loop 子进程loop

[[noreturn]] void ngx_master_process_loop();
void ngx_fork_process(int processNums, const char * processName);
void ngx_work_process_init(const char * processName);

[[noreturn]] void ngx_work_process_loop();

#endif //JM_NGINX_NGX_PROC_H
