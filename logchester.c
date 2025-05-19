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
    int innerCounter = 0;
    int repeat = -1;
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
    for(int i = entry->length - 1; i >= 0 ; i--)
    {

        


        record_type = data[i].ut_type;

        if(strcmp(data[i].ut_user , "shutdown") == 0)
        {
            endCounter = 1;
            while((i - endCounter) >= 0 && data[i - endCounter].ut_type != 1)
            {
                    
                if(strcmp(data[i - endCounter].ut_host , "login screen")== 0 && (i - endCounter) != repeat)
                {
                    session_time_info = show_time((time_t)data[i - endCounter].tv_sec , 0);

                    printf("%-12s%-12s%-27s%-17s- %-8s%-30s\n", 
                            data[i - endCounter].ut_user,
                            data[i - endCounter].ut_line,
                            data[i - endCounter].ut_host,
                            session_time_info->login_time,
                            "Down",
                            record_types[data[i - endCounter].ut_type]);
                    repeat = i - endCounter;
    
                    free(session_time_info);
                }
                else if(data[i - endCounter].ut_type == 8 )
                {
                    innerCounter = 1;
                    while((i - endCounter - innerCounter) >= 0 && (i - endCounter - innerCounter) != repeat)
                    {
                                
                    if((strcmp(data[i - endCounter].ut_line , data[i - endCounter - innerCounter].ut_line) == 0 && 
                        strcmp(data[i - endCounter].ut_host , data[i - endCounter - innerCounter].ut_host) == 0 )
                      ||
                        (strcmp(data[i - endCounter].ut_line , data[i - endCounter - innerCounter].ut_line) == 0 && 
                         strcmp(data[i - endCounter].ut_user , data[i - endCounter - innerCounter].ut_user) == 0))
                      
                        {
                            session_time_info = show_time((time_t)data[i - endCounter - innerCounter].tv_sec , (time_t)data[i - endCounter].tv_sec );
                            printf("%-12s%-12s%-27s%-17s- %-8s%-30s\n",
                                data[i - endCounter - innerCounter].ut_user,
                                data[i - endCounter - innerCounter].ut_line,
                                data[i - endCounter - innerCounter].ut_host,
                                session_time_info->login_time,
                                session_time_info->logout_time,
                                record_types[data[i - endCounter - innerCounter].ut_type]);
                            repeat = i - endCounter - innerCounter;
    
                                break;
                         }
                    
                        innerCounter++;
                    }
                }
                
                else if(data[i - endCounter].ut_type == 7 && (i - endCounter) != repeat) 
                {
                            session_time_info = show_time((time_t)data[i - endCounter].tv_sec , (time_t)data[i].tv_sec );
                            printf("%-12s%-12s%-27s%-17s- %-8s%-30s\n",
                                data[i - endCounter].ut_user,
                                data[i - endCounter].ut_line,
                                data[i - endCounter].ut_host,
                                session_time_info->login_time,
                                session_time_info->logout_time,
                                record_types[data[i - endCounter].ut_type]);
                            repeat = i - endCounter;


                }

                
                endCounter++;
            }
        }

        
        
    }

    

    // Freeing memory to prevent memory leaks.
    free(entry->data);
    free(entry);

    return 0;
}

