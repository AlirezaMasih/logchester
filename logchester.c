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
    struct session_data *data = entry->data;
    struct session_time *session_time_info;
    

    //char time_buffer[17]; 
    short record_type;
    int endCounter = 0;
    uint32_t time_offset;
 
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

        


        record_type = data[i].ut_type;

        if(record_type < 8)

        {

            session_time_info = show_time((time_t)data[i].tv_sec , 0 , 0);
            printf("%-12s%-12s%-27s%-17s- %-8s %-15s%-30s\n" , 
                    data[i].ut_user ,
                    data[i].ut_line ,
                    data[i].ut_host ,
                    session_time_info->login_time ,
                    session_time_info->logout_time,
                    session_time_info->login_duration ,
                    record_types[record_type]);               
                    free(session_time_info);
        }
        
        else if(record_type == 8)
        {
            endCounter = 1;
            while(i - endCounter >= 0 )
            {
                if(strcmp(data[i].ut_line , data[i - endCounter].ut_line) == 0)
                {
                    time_offset = data[i].tv_sec - data[i - endCounter].tv_sec;
                    session_time_info = show_time((time_t)data[i - endCounter].tv_sec , (time_t)data[i].tv_sec , (time_t)time_offset); 

                    break;
                }
                else
                {
                    endCounter++;
                }
            }
        }




        
    }



    // Freeing memory to prevent memory leaks.
    free(entry->data);
    free(entry);

    return 0;
}

