#include "read_config.h"

int read_config(const char *filename , struct config_data *cfg)
{

    FILE *config_file = fopen(filename , "r");

    if(!config_file)
    {
        perror("can't find logchester.conf file");
        return -1;
    }

    char line_buffer[256];

    while(fgets(line_buffer , sizeof(line_buffer) , config_file))
    {

        


        if(strncmp(line_buffer , "#" , 1) == 0)
        {
            continue;
        }

        line_buffer[strcspn(line_buffer , "\n" )] = '\0';
        trim(line_buffer);

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
        else if(strncmp(line_buffer , "remote_file=" , 12) == 0)
        {
            strcpy(cfg->remote_file , line_buffer + 12);
        }
        else if(strncmp(line_buffer , "log_file=" , 9) == 0)
        {
            strcpy(cfg->log_file , line_buffer + 9);
        }
    }

    fclose(config_file);
    return 0;
    
}

void trim(char *str)
{
    char *src = str , *dst = str;

    while(*src)
    {
        if(!isspace((unsigned char)*src))
        {
            *dst++=*src;
        }
        src++;
    }
    *dst=0;
}
