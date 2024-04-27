//
// Created by jm on 4/27/24.
//

#ifndef JM_NGINX_NGX_GLOBAL_H
#define JM_NGINX_NGX_GLOBAL_H
#include <cstddef>
typedef struct {
    char ItemName[50];
    char ItemContent[500];
}ConfItem, *ConfItemPtr;

extern char *g_environ;
extern size_t g_environLen;
extern char ** g_os_argv;
#endif //JM_NGINX_NGX_GLOBAL_H
