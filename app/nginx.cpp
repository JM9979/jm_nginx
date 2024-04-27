//
// Created by jm on 4/23/24.
//
#include <iostream>
#include <unistd.h>
#include <cstring>
#include "../include/ngx_conf.h"
#include "../include/signal.h"
#include "../include/ngx_func.h"

const char *ConfName = "../nginx.conf";
char *g_environ = nullptr;
size_t g_environLen = 0;
char **g_os_argv = nullptr;


int main(int argc, char * const *argv) {
    g_os_argv = (char **)argv;
    std::cout << "Hello jm_nginx" << std::endl;
    ngx_conf::getInstance().load(ConfName);
    std::cout << "ListenPort = " << ngx_conf::getInstance().getInt("ListenPort") << std::endl;
    std::cout << "IPAddress = " << ngx_conf::getInstance().getString("IPAddress") << std::endl;

/*
    char *cht[31];
    for(size_t i = 0; environ[i] != nullptr; ++i) {
        std::cout << environ[i] << std::endl;
        cht[i] = environ[i];
    }
    std::cout << "--------------------------------------" << std::endl;
*/
    ngx_func::ngx_setProcTitle_init();
/*
    char *myCty[31];
    for(size_t i = 0; environ[i] != nullptr; ++i) {
        std::cout << environ[i] << std::endl;
        myCty[i] = environ[i];
    }
*/
    ngx_func::ngx_setProTitle("nginx: Process Master!!!!");
    if(g_environ != nullptr) {
        delete[] g_environ;
        g_environ = nullptr;
    }
//    while(1);
    return 0;
}