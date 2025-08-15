#pragma once // Ensures this header is included only once during compilation.

// Core SSH functionality (sessions, authentication, etc.)
#include <libssh2.h>

// Standard library (e.g., memory allocation, exit codes)
#include <stdlib.h>

// For socket creation and network communication
#include <sys/socket.h>

// SFTP support using libssh2
#include <libssh2_sftp.h>

// Definitions for network address structures (IPv4)
#include <netinet/in.h>

// Functions for converting IP addresses (e.g., inet_addr)
#include <arpa/inet.h>

// For file handling (open, read, etc.)
#include <fcntl.h>

// Project-specific configuration struct and parser
#include "read_config.h"

// Sends a file via SSH/SFTP based on the provided configuration
int send_file(struct config_data *cfg , char *local_file , char *remote_file);

