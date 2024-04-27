//
// Created by jm on 4/23/24.
//

#ifndef JM_NGINX_NGX_CONF_H
#define JM_NGINX_NGX_CONF_H
#include "ngx_global.h"
#include <vector>
// 单例模式实现
// bool load() 加载配置文件
// const char *getString() 获取配置项中的字符串
// int getInt() 获取整数
// vector<ConfItem> m_ConfigItemList; 配置文件中的列表项

class ngx_conf {
private:
    ngx_conf() {}
public:
    ngx_conf(const ngx_conf& other) = delete;
    ngx_conf& operator=(const ngx_conf& other) = delete;

    static ngx_conf& getInstance();
    bool load(const char *ConfName);
    const char *getString(const char * name);
    int getInt(const char * name, int def = 80);
private:
    std::vector<ConfItem> m_ConfigItemList;
};


#endif //JM_NGINX_NGX_CONF_H
