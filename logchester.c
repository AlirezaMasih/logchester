#include "session_process.h"  // Header file for session processing functions
#include "send_data.h"         // Header file for sending data to a server
#include "read_config.h"

#include <sys/inotify.h>       // Library for file system event monitoring
#include <unistd.h>            // Header for standard symbolic constants and types

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

int main()
{
    struct config_data cfg;

    int rc = read_config(config_file , &cfg);

    int fd, wd, length;   // File descriptor for inotify, watched file descriptor, and length of the data read

    char buffer[sizeof(struct inotify_event) + 16];  // Buffer for storing inotify events

    // Initialize inotify for monitoring changes to the /var/log/wtmp file
    fd = inotify_init();  
    wd = inotify_add_watch(fd, cfg.utmp_path , IN_MODIFY);  // Watch for modifications to /var/log/wtmp

    printf("Started \n");  // Print a message indicating monitoring has started
                           //
    cfg.auth_status = 1;
    while(1)  // Infinite loop for continuous monitoring
    {
        // Read events from inotify
        length = read(fd, buffer, sizeof(buffer));

        if(length > 0)  // If data was read
        {

            // Read the data from the wtmp file and store it in the structure
            struct utmp_data *entry = read_file(cfg.utmp_path);
            
            // Handle the session data (e.g., login and logout processing)
            session_handle(entry , &cfg);

            // Send the processed data to the server
            
            if(strlen(cfg.ip) >= 7)
            {
                send_file(&cfg);
            }
        }
        
    }
    
    // Remove the watch on the file and close the inotify file descriptor
    inotify_rm_watch(fd, wd);  
    close(fd);  // Close the inotify file descriptor

    return 0;  // End of the program
}

