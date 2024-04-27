//
// Created by jm on 4/27/24.
//
#include <unistd.h>
#include <cstring>
#include "ngx_func.h"
#include "ngx_global.h"
// main函数传入参数与环境变量所在的内存是连续的, 考虑将环境变量的内存移动, 防止在重新设置进程名的时候将环境变量内存覆盖掉
// ngx_setProcTitle_init() 移动环境变量的内存
// ngx_setProcTitle()  设置环境变量

void ngx_func::ngx_setProcTitle_init() {
    // 计算环境变量一共有多少内存
    size_t len = 0;
    for(size_t i = 0; environ[i] != nullptr; ++i) {
        len += strlen(environ[i]) + 1;
    }
    g_environLen = len;

    // 申请一块新的内存存储环境变量
    g_environ = new char[len];
    char *ptr = g_environ;

    for(size_t i = 0; environ[i] != nullptr; ++i) {
        size_t newLen = strlen(environ[i]) + 1;
        strncpy(ptr, environ[i], newLen);
        environ[i] = ptr;
        ptr += newLen;
    }
}

bool ngx_func::ngx_setProTitle(const char *title) {
    size_t titleLen = strlen(title) + 1;
    size_t len = g_environLen;
    for(size_t i = 0; g_os_argv[i] != nullptr; ++i) {
        len += strlen(g_os_argv[i]) + 1;
    }
    if(titleLen > len) {
        return false;
    }
    char *t_temp = g_os_argv[0];
    g_os_argv[1] = nullptr;
    // 将标题放置好, 同时将多余的内容赋值为'\0'
    strncpy(t_temp, title, len);
    return true;
}