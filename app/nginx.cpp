//
// Created by jm on 4/23/24.
//
#include <iostream>
#include "../include/ngx_conf.h"
#include "../include/signal.h"

int main() {
    std::cout << "Hello jm_nginx" << std::endl;
    ngx_conf a;
    a.ngx_conf_test();
    signal b;
    b.signal_test();
    return 0;
}