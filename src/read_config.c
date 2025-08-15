#include "../include/read_config.h"

// Reads the configuration file and populates the config_data structure
int read_config(const char *filename , struct config_data *cfg)
{
    // Try to open the config file in read mode
    FILE *config_file = fopen(filename , "r");

    if(!config_file)
    {
        // If the file couldn't be opened, print an error and return -1
        perror("can't find logchester.conf file");
        return -1;
    }

    char line_buffer[256];  // Buffer to hold each line from the config file

    // Read the file line-by-line
    while(fgets(line_buffer , sizeof(line_buffer) , config_file))
    {
        // Skip lines starting with '#' (comments)
        if(strncmp(line_buffer , "#" , 1) == 0)
        {
            continue;
        }

        // Remove the trailing newline character
        line_buffer[strcspn(line_buffer , "\n" )] = '\0';

        // Remove all whitespace from the line
        trim(line_buffer);

        // Match and parse each config option
        if(strncmp(line_buffer , "utmp_path=" , 10) == 0)
        {
            strcpy(cfg->utmp_path , line_buffer + 10);
        }
        else if(strncmp(line_buffer , "ip=" , 3) == 0)
        {
            strcpy(cfg->ip , line_buffer + 3);
        }
        else if(strncmp(line_buffer , "username=" , 9) == 0)
        {
            strcpy(cfg->username , line_buffer + 9);
        }
        else if(strncmp(line_buffer , "password=" , 9) == 0)
        {
            strcpy(cfg->password , line_buffer + 9);
        }
        else if(strncmp(line_buffer , "remote_login_logs=" , 18) == 0)
        {
            strcpy(cfg->remote_login_logs , line_buffer + 18);
        }
        else if(strncmp(line_buffer , "login_logs=" , 11) == 0)
        {
            strcpy(cfg->login_logs , line_buffer + 11);
        }
        else if(strncmp(line_buffer , "pub_key=" , 8) == 0)
        {
            strcpy(cfg->public_key , line_buffer + 8);
        }
        else if(strncmp(line_buffer , "pri_key=" , 8) == 0)
        {
            strcpy(cfg->private_key , line_buffer + 8);
        }
        else if(strncmp(line_buffer , "passphrase=" , 11) == 0)
        {
            strcpy(cfg->passphrase , line_buffer + 11);
        }
        else if(strncmp(line_buffer , "socket_path=" , 12) == 0)
        {
            strcpy(cfg->socket_path , line_buffer+12);
        }
        else if(strncmp(line_buffer , "remote_logs=" , 12) == 0)
        {
            strcpy(cfg->remote_logs , line_buffer+12);
        }
        else if(strncmp(line_buffer , "logs=" , 5) == 0)
        {
            strcpy(cfg->logs , line_buffer + 5);
        }
    }

    // Close the config file
    fclose(config_file);
    return 0;
}

// Removes all whitespace characters from a string (in-place)
void trim(char *str)
{
    char *src = str , *dst = str;

    while(*src)
    {
        // Copy non-whitespace characters
        if(!isspace((unsigned char)*src))
        {
            *dst++ = *src;
        }
        src++;
    }
    *dst = 0;  // Null-terminate the trimmed string
}

