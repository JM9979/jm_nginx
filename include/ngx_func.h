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
    // 去除字符串中的空白字符
    static ngx_func& getInstance();
    void eraseEmpty(char *string);
};


#endif //JM_NGINX_NGX_FUNC_H
