//
// Created by jm on 4/23/24.
//
#include <iostream>
#include "../include/ngx_conf.h"
#include "../include/signal.h"

const char *ConfName = "../nginx.conf";

int main() {
    std::cout << "Hello jm_nginx" << std::endl;
    ngx_conf::getInstance().load(ConfName);
    std::cout << "ListenPort = " << ngx_conf::getInstance().getInt("ListenPort") << std::endl;
    std::cout << "IPAddress = " << ngx_conf::getInstance().getString("IPAddress") << std::endl;
    signal b;
    b.signal_test();
    return 0;
}