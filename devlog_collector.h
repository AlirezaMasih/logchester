#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>


struct devlog_data
{
    struct sockaddr_un sender_addr;
    socklen_t addr_len;
    int socket_fd;
};


struct devlog_data *devlog_collector(char *socket_path);
void receive_logs(struct devlog_data *dev_data , char *devlog_buffer);
