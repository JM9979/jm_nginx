//
// Created by jm on 5/5/24.
//

#include "DaemonProc.h"
#include "log.h"

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

bool Daemon() {
    switch(fork()) {
        case -1:
            LOG_ERROR << "daemon() error" << std::endl;
            return false;
        case 0:
            break;
        default:
            exit(0);
    }

    if(setsid() == -1) {
        LOG_ERROR << "daemon() setsid() errno" << std::endl;
        return false;
    }

    if(umask(0) == -1) {
        LOG_ERROR << "daemon() umask() errno" << std::endl;
        return false;
    }

    int fd = open("/dev/null", O_RDWR);
    if(fd == -1) {
        LOG_ERROR << "daemon() open() errno" << std::endl;
        return false;
    }
    if(dup2(fd, STDIN_FILENO) == -1) {
        LOG_ERROR << "daemon() STDIN_FILENO" << std::endl;
        return false;
    }
    if(dup2(fd, STDOUT_FILENO) == -1) {
        LOG_ERROR << "daemon() STDOUT_FILENO" << std::endl;
        return false;
    }
    if(fd > STDERR_FILENO) {
        if(close(fd) == -1) {
            LOG_ERROR << "daemon() close fd" << std::endl;
            return false;
        }
    }
    return true;
}