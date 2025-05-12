#include "collector.h"

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
    // Read the data from the utmp file and store it in the structure
    struct utmp_data *entry = read_file(_PATH_WTMP);
    struct utmp *data = entry->data;
    

    char time_buffer[17]; 
    short record_type;
    int time_offset;
 
    char record_types[9][14] = {
        "EMPTY",         // 0
        "RUN_LVL",       // 1
        "BOOT_TIME",    // 2
        "NEW_TIME",      // 3
        "OLD_TIME",      // 4
        "INIT_PROCESS",  // 5
        "LOGIN_PROCESS", // 6
        "USER_PROCESS", // 7
        "DEAD_PROCESS"  // 8
    };
    // Loop to print the information of each entry in the data
    for(int i = 0; i < entry->length; i++)
    {

        show_time(&data[i].ut_tv.tv_sec , time_buffer , sizeof(time_buffer));


        record_type = data[i].ut_type;

        printf("%-10s%-8s%-25s%-25s%-30s\n" , data[i].ut_user , data[i].ut_line , data[i].ut_host , time_buffer , record_types[record_type]);

       
        
    }



    // Freeing memory to prevent memory leaks.
    free(entry->data);
    free(entry);

    return 0;
}

