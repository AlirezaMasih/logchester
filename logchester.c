#include "session_process.h"  // Header file for session processing functions
#include "send_data.h"        // Header file for sending data to a server
#include "read_config.h"      // Header for reading configuration file
#include "devlog_collector.h" // Header for device log collection

#include <sys/inotify.h>      // Library for file system event monitoring
#include <unistd.h>           // Header for standard symbolic constants and types
#include <pthread.h>          // For using threads

/*
    Definition of data types (currently commented out):
    #define Empty 0
    #define RunLevel 1
    #define BootTime 2
    #define NewTime 3
    #define OldTime 4
    #define InitProcess 5
    #define LoginProcess 6
    #define UserProcess 7
    #define DeadProcess 8
*/

#define config_file "logchester.conf" // Name of the configuration file

// Thread function to handle device log collection
void *devlog_handle(void *cfg)
{
    struct config_data *conf = (struct config_data *)cfg;

    char devlog_buffer[4096]; // Buffer for device log messages
    struct devlog_data *dev_data = devlog_collector(conf->socket_path); // Initialize device log collector
    int receive_status = 0;

    while(1)
    {

        receive_status = receive_logs(dev_data , devlog_buffer , conf); // Continuously receive and print device logs
        if (receive_status == 1)
        {
            // Send the processed data to the server if IP is set
            send_file(conf , conf->logs , conf->remote_logs);
            receive_status = 0;
        }

    }

    free(dev_data); // Free allocated memory (unreachable in this infinite loop)
}

// Thread function to handle utmp file monitoring and session processing
void *utmp_handle(void *cfg)
{
    struct config_data *conf = (struct config_data *)cfg;

    int fd, wd, length;   // File descriptor for inotify, watch descriptor, and length of data read

    char buffer[sizeof(struct inotify_event) + 16];  // Buffer for storing inotify events

    // Initialize inotify for monitoring changes to the utmp file
    fd = inotify_init();  
    wd = inotify_add_watch(fd, conf->utmp_path , IN_MODIFY);  // Watch for modifications

    printf("Started \n");  // Indicate that monitoring has started

    conf->auth_status = 1; // Set authentication status

    while(1)  // Infinite loop for continuous monitoring
    {
        // Read events from inotify
        length = read(fd, buffer, sizeof(buffer));

        if(length > 0)  // If data was read (file was modified)
        {
            // Read the data from the utmp file and store it in the structure
            struct utmp_data *entry = read_file(conf->utmp_path);
            
            // Handle the session data (e.g., login and logout processing)
            session_handle(entry , conf);

            // Send the processed data to the server if IP is set
            if(strlen(conf->ip) >= 7)
            {
                send_file(conf , conf->login_logs , conf->remote_login_logs);
            }
        }
    }
    
    // Remove the watch on the file and close the inotify file descriptor (unreachable code)
    inotify_rm_watch(fd, wd);  
    close(fd);  
}

// Main function: entry point of the program
int main()
{
    struct config_data cfg; // Configuration data structure
    int rc = read_config(config_file , &cfg); // Read configuration from file
    
    pthread_t devlog_thread; // Thread for device log handling
    pthread_t utmp_thread;   // Thread for utmp file monitoring

    // Create threads for device log and utmp handling
    pthread_create(&devlog_thread , NULL , devlog_handle , (void *)&cfg);
    pthread_create(&utmp_thread , NULL , utmp_handle , (void *)&cfg);

    // Wait for both threads to finish (in practice, they run infinitely)
    pthread_join(devlog_thread , NULL);
    pthread_join(utmp_thread , NULL);

    return 0;  // End of the program
}
