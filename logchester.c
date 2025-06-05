#include "session_process.h"
#include "send_data.h"

#include <sys/inotify.h>
#include <unistd.h>

/*
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

int main()
{
    int fd , wd , length;

    char buffer[sizeof(struct inotify_event) + 16];

    fd = inotify_init();
    wd = inotify_add_watch(fd , "/var/log/wtmp" , IN_MODIFY);

    printf("Started \n");
    while(1)
    {
        length = read(fd , buffer , sizeof(buffer));

        if(length > 0)
        {

            // Read the data from the utmp file and store it in the structure

            struct utmp_data *entry = read_file(_PATH_WTMP);
            session_handle(entry);

            send_file();

        }
        
    }
    inotify_rm_watch(fd , wd);
    close(fd);

    return 0;
}

