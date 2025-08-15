#pragma once // Ensures this header is included only once during compilation.

#include <stdio.h>        // For standard input/output functions
#include <string.h>       // For string manipulation functions
#include <stdlib.h>       // For memory allocation and general utilities
#include <sys/socket.h>   // For socket programming (socket, bind, etc.)
#include <sys/un.h>       // For UNIX domain sockets (struct sockaddr_un)
#include <unistd.h>       // For POSIX API (unlink, etc.)
#include "read_config.h"  // For config_data structure

// Structure to hold information about the device log socket
struct devlog_data
{
    struct sockaddr_un sender_addr; // Address structure for the sender
    socklen_t addr_len;             // Length of the address structure
    int socket_fd;                  // File descriptor for the socket
};

// Initializes the device log collector and returns a pointer to devlog_data
struct devlog_data *devlog_collector(char *socket_path);

// Receives logs from the device and stores them in the provided buffer
int receive_logs(struct devlog_data *dev_data , char *devlog_buffer , struct config_data *cfg);
