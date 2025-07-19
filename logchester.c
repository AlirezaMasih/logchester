#include "session_process.h"  // Header file for session processing functions
#include "send_data.h"         // Header file for sending data to a server
#include "read_config.h"
#include "devlog_collector.h"

#include <sys/inotify.h>       // Library for file system event monitoring
#include <unistd.h>            // Header for standard symbolic constants and types
#include <pthread.h>

/*
    Definition of data types which are currently commented out:
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

#define config_file "logchester.conf"

void *devlog_handle(void *cfg)
{
    char devlog_buffer[4096];
    struct devlog_data *dev_data = devlog_collector("/home/masih/test_socket");

    while(1)
    {
        receive_logs(dev_data , devlog_buffer);
    }
    free(dev_data);

}

void *utmp_handle(void *cfg)
{

    struct config_data *conf = (struct config_data *)cfg;

    int fd, wd, length;   // File descriptor for inotify, watched file descriptor, and length of the data read

    char buffer[sizeof(struct inotify_event) + 16];  // Buffer for storing inotify events
       // Initialize inotify for monitoring changes to the /var/log/wtmp file
    fd = inotify_init();  
    wd = inotify_add_watch(fd, conf->utmp_path , IN_MODIFY);  // Watch for modifications to /var/log/wtmp

    printf("Started \n");  // Print a message indicating monitoring has started
                           //
    conf->auth_status = 1;
    while(1)  // Infinite loop for continuous monitoring
    {
        // Read events from inotify

        length = read(fd, buffer, sizeof(buffer));

        if(length > 0)  // If data was read
        {

            // Read the data from the wtmp file and store it in the structure
            struct utmp_data *entry = read_file(conf->utmp_path);
            
            // Handle the session data (e.g., login and logout processing)
            session_handle(entry , conf);

            // Send the processed data to the server
            
            if(strlen(conf->ip) >= 7)
            {
                send_file(conf);
            }
        }
        
    }
    
    // Remove the watch on the file and close the inotify file descriptor
    inotify_rm_watch(fd, wd);  
    close(fd);  // Close the inotify file descriptor



}

int main()
{

    struct config_data cfg;
    int rc = read_config(config_file , &cfg);
    
    pthread_t devlog_thread;
    pthread_t utmp_thread;

    pthread_create(&devlog_thread , NULL , devlog_handle , (void *)&cfg);
    pthread_create(&utmp_thread , NULL , utmp_handle , (void *)&cfg);


    pthread_join(devlog_thread , NULL);
    pthread_join(utmp_thread , NULL);





    return 0;  // End of the program
}

