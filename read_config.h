#pragma once 

#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct config_data
{
    char utmp_path[128];
    char ip[20];
    char username[128];
    char password[128];
    char send_to_server[3];
    char remote_file[128];
    char log_file[128];
    char public_key[128];
    char private_key[128];
    char passphrase[64];
    int auth_status;
};


void trim(char *str);
int read_config(const char *filename , struct config_data *cfg);
