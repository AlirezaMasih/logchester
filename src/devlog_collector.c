#include "../include/devlog_collector.h"

// Function to initialize and return a devlog_data structure for device log collection
struct devlog_data *devlog_collector(char *socket_path)
{
    struct devlog_data *dev_data = malloc(sizeof(struct devlog_data)); // Allocate memory for devlog_data

    int socket_fd = socket(AF_UNIX , SOCK_DGRAM , 0); // Create a UNIX domain datagram socket
    struct sockaddr_un log_socket;    // Address structure for the log socket
    struct sockaddr_un sender_addr;   // Address structure for the sender

    memset(&log_socket , 0 , sizeof(struct sockaddr_un)); // Zero out the log_socket structure

    log_socket.sun_family = AF_UNIX;                  // Set address family to UNIX
    strcpy(log_socket.sun_path , socket_path);         // Set the socket path

    unlink(socket_path); // Remove any existing socket file at the path
    bind(socket_fd , (struct sockaddr *)&log_socket , sizeof(struct sockaddr_un)); // Bind the socket

    dev_data->socket_fd = socket_fd;          // Store socket file descriptor
    dev_data->sender_addr = sender_addr;      // Store sender address structure
    dev_data->addr_len = sizeof(struct sockaddr_un); // Store address length

    return dev_data; // Return the initialized devlog_data structure
}

// Function to receive logs from the device and print them
int receive_logs(struct devlog_data *dev_data , char *devlog_buffer , struct config_data *cfg)
{
    // Receive a datagram from the socket
    ssize_t received = recvfrom(dev_data->socket_fd , devlog_buffer , 4096 , 0 , (struct sockaddr *)&dev_data->sender_addr , &dev_data->addr_len);

    FILE *local_logs = fopen(cfg->logs , "a");
    if(received > 0)
    {
        devlog_buffer[received] = '\0';      // Null-terminate the received data
        fprintf(local_logs , "%s" , devlog_buffer);      // Print the log message
        memset(devlog_buffer , 0 , 4096);    // Clear the buffer for the next message
        fclose(local_logs);
        return 1;
    }
}
