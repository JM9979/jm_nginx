//
// Created by jm on 4/23/24.
//

#include "../include/ngx_conf.h"
#include "../include/ngx_func.h"
#include <iostream>
#include <cstring>

ngx_conf& ngx_conf::getInstance() {
    static ngx_conf instance;
    return instance;
}

bool ngx_conf::load(const char *ConfName) {
    FILE *fp = fopen(ConfName, "r");
    if(fp == nullptr) {
        return false;
    }
    char buf[500];
    while(fgets(buf, 500, fp) != nullptr) {
        // 去除字符串中的空格
        ngx_func::getInstance().eraseEmpty(buf);
        // 无效行
        if(buf[0] == '\n' || buf[0] == '\t' || buf[0] == '\0' || buf[0] == '#' || buf[0] == '[' || buf[0] == ';') {
            continue;
        }

        char *p1 = strchr(buf, '=');
        char *p2 = strchr(buf, ';');
        if(p1 != nullptr && p2 != nullptr) {
            ConfItem Item;
            size_t nameLen = p1 - buf;
            size_t contentLen = p2 - p1 -1;
            strncpy(Item.ItemName, buf, nameLen);
            Item.ItemName[nameLen] = '\0';
            strncpy(Item.ItemContent, p1 + 1, contentLen);
            Item.ItemContent[contentLen] = '\0';
            m_ConfigItemList.emplace_back(Item);
           } else {
            fclose(fp);
            return false;
        }
    }
    fclose(fp);
    return true;
}

const char *ngx_conf::getString(const char * name) {

    for(const auto &iter : m_ConfigItemList) {
        if(strcasecmp(iter.ItemName, name) == 0) {
            return iter.ItemContent;
        }
    }
    return nullptr;
}

int ngx_conf::getInt(const char *name, const int def) {
    for(const auto &iter : m_ConfigItemList) {
        if(strcasecmp(iter.ItemName, name) == 0) {
            return atoi(iter.ItemContent);
        }
    }
    return def;
}