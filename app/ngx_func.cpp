//
// Created by jm on 4/27/24.
//

#include "ngx_func.h"
#include <cstring>


ngx_func& ngx_func::getInstance() {
    static ngx_func instance;
    return instance;
}

void ngx_func::eraseEmpty(char *string) {
    if(string == nullptr) {
        return ;
    }

    size_t slow = 0, fast = 0;
    while(string[fast] != 0) {
        if(string[fast] != ' ') {
            string[slow++] = string[fast++];
        } else {
            fast++;
        }
    }
    string[slow] = 0;
}