//
// Created by jm on 4/27/24.
//

#ifndef JM_NGINX_NGX_FUNC_H
#define JM_NGINX_NGX_FUNC_H


class ngx_func {
private:
    ngx_func() {}
public:
    ngx_func(const ngx_func& other) = delete;
    ngx_func& operator=(const ngx_func& other) = delete;

    static ngx_func& getInstance();
    // 去除字符串中的空白字符
    void eraseEmpty(char *string);
    // 移动环境变量的内存
    static void ngx_setProcTitle_init();
    // 重命名进程名称
    static bool ngx_setProTitle(const char *title);
};


#endif //JM_NGINX_NGX_FUNC_H
