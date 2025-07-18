#include "devlog_collector.h"


struct devlog_data *devlog_collector(char *socket_path)
{
    struct devlog_data *dev_data = malloc(sizeof(struct devlog_data));

    int socket_fd = socket(AF_UNIX , SOCK_DGRAM , 0);
    struct sockaddr_un log_socket;
    struct sockaddr_un sender_addr;

    memset(&log_socket , 0 , sizeof(struct sockaddr_un));

    log_socket.sun_family = AF_UNIX;
    strcpy(log_socket.sun_path , socket_path);

    unlink(socket_path);
    bind(socket_fd , (struct sockaddr *)&log_socket , sizeof(struct sockaddr_un));

    dev_data->socket_fd = socket_fd;
    dev_data->sender_addr = sender_addr;
    dev_data->addr_len = sizeof(struct sockaddr_un);

    return dev_data;

}

void receive_logs(struct devlog_data *dev_data , char *devlog_buffer)
{

    ssize_t received = recvfrom(dev_data->socket_fd , devlog_buffer , 4096 , 0 , (struct sockaddr *)&dev_data->sender_addr , &dev_data->addr_len);
    if(received > 0)
        {
            devlog_buffer[received] = '\0';
            printf("%s\n" , devlog_buffer);
            memset(devlog_buffer , 0 , 4096);

        }

}
