//
// Created by jm on 4/23/24.
//

#include "../include/ngx_conf.h"
#include <iostream>
int ngx_conf::ngx_conf_test() {
    std::cout << "this is ngx_conf" << std::endl;
    return 0;
}