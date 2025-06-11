#pragma once // This directive ensures the header file is included only once during compilation (header guard).

#include <stdio.h>    // For input/output functions (like printf, fopen, etc.)
#include <string.h>   // For string manipulation functions (like strcpy, strncpy, etc.)
#include <ctype.h>    // For character handling functions (like isspace, etc.)

// Structure to store configuration data
struct config_data
{
    char utmp_path[128];     // Path to the utmp file to monitor (stores user login information)
    char ip[20];             // IP address of the SSH server to connect to
    char username[128];      // Username for authentication
    char password[128];      // Password for authentication
    char send_to_server[3];  // Flag for sending data to the server (mostly used for status tracking)
    char remote_file[128];   // Path of the file on the remote server to send data to
    char log_file[128];      // Path of the local file to be sent
    char public_key[128];    // Path to the public key for key-based authentication
    char private_key[128];   // Path to the private key for key-based authentication
    char passphrase[64];     // Passphrase for the private key, if required
    int auth_status;         // Authentication status (to track the status of public key and password)
};

// Function to remove leading and trailing whitespace from a string
void trim(char *str);

// Function to read and parse the configuration file
// It reads the data from the specified file and stores it in the config_data structure
int read_config(const char *filename , struct config_data *cfg);

