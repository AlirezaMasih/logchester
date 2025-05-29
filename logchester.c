#include "collector.h"
#include "datetime.h"

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
    
    // Declare variables
    int endCounter = 0;
    int innerCounter = 0;
    uint32_t time_offset;
 
    // Array holding different record types as strings for easier reference
    char record_types[9][14] = {
        "EMPTY",         // 0
        "RUN_LVL",       // 1
        "BOOT_TIME",     // 2
        "NEW_TIME",      // 3
        "OLD_TIME",      // 4
        "INIT_PROCESS",  // 5
        "LOGIN_PROCESS", // 6
        "USER_PROCESS",  // 7
        "DEAD_PROCESS"   // 8
    };

    for(int i = 0; i < entry->length; i++)
    {
        // Get the record type for the current entry

        // If the record is of type BOOT_TIME (2)
        if(data[i].ut_type == 2)
        {   
           endCounter = 1;
           
           // Loop to handle entries following the current one, looking for a matching or shutdown entry
           while((i + endCounter) <= entry->length)
           {
               if(i + endCounter == entry->length)
               {
                    // Call show_time with 0 (no crash)
                    session_time_info = show_time((time_t)data[i].tv_sec , (time_t)data[i + endCounter].tv_sec , 0);

                    // Print formatted information
                    printf("%-12s%-12s%-27s%-17s- %-8s%-30s\n",
                            data[i].ut_user,
                            data[i].ut_line,
                            data[i].ut_host,
                            session_time_info->login_time,
                            session_time_info->logout_time,
                            record_types[data[i].ut_type]);
                    free(session_time_info);  // Free allocated memory
                    break;
               }

               // If the next entry is a BOOT_TIME (crash)
               else if(data[i + endCounter].ut_type == 2)
               {
                    session_time_info = show_time((time_t)data[i].tv_sec , (time_t)data[i + endCounter].tv_sec , 1); // 1 means crash

                    // Print formatted information
                    printf("%-12s%-12s%-27s%-17s- %-8s%-30s\n",
                            data[i].ut_user,
                            data[i].ut_line,
                            data[i].ut_host,
                            session_time_info->login_time,
                            session_time_info->logout_time,
                            record_types[data[i].ut_type]);
                    free(session_time_info);  // Free allocated memory
                    break;
                }

               // If the next entry is a shutdown process
               else if(strcmp(data[i + endCounter].ut_user , "shutdown") == 0)
               {
                    session_time_info = show_time((time_t)data[i].tv_sec , (time_t)data[i + endCounter].tv_sec , 4);
        
                    // Print formatted information
                    printf("%-12s%-12s%-27s%-17s- %-8s%-30s%-30s\n",
                            data[i].ut_user,
                            data[i].ut_line,
                            data[i].ut_host,
                            session_time_info->login_time,
                            session_time_info->logout_time,
                            session_time_info->login_duration,
                            record_types[data[i].ut_type]);
                    free(session_time_info);  // Free allocated memory
                    break;
               } 
 
                endCounter++;
               
           }

        }
        
        // If the record is of type USER_PROCESS (7)
        else if(data[i].ut_type == 7)
        {          
            endCounter = 1;

            // Loop to handle subsequent entries and identify end of user session
            while((i + endCounter) <= entry->length)
            {
                if((i + endCounter) == entry->length)
                {
                    // Call show_time with 0 (still logged in)
                    session_time_info = show_time((time_t)data[i].tv_sec , (time_t)data[i + endCounter].tv_sec , 0);
                    printf("%-12s%-12s%-27s%-17s- %-8s%-30s%-30s\n",
                            data[i].ut_user,
                            data[i].ut_line,
                            data[i].ut_host,
                            session_time_info->login_time,
                            session_time_info->logout_time,
                            session_time_info->login_duration,
                            record_types[data[i].ut_type]);
                    free(session_time_info);  // Free allocated memory
                    break;
                }
                // If the next entry is a shutdown process
                else if(strcmp(data[i + endCounter].ut_user , "shutdown") == 0)
                {
                    session_time_info = show_time((time_t)data[i].tv_sec , (time_t)data[i + endCounter].tv_sec , 2); // 2 means down

                    // Print formatted information
                    printf("%-12s%-12s%-27s%-17s- %-8s%-30s%-30s\n",
                            data[i].ut_user,
                            data[i].ut_line,
                            data[i].ut_host,
                            session_time_info->login_time,
                            session_time_info->logout_time,
                            session_time_info->login_duration,
                            record_types[data[i].ut_type]);
                    free(session_time_info);  // Free allocated memory
                    break;
                }
                // If the next record is of type DEAD_PROCESS (8)
                else if(data[i + endCounter].ut_type == 8)
                {                    
                    // Check if the user, line, or host match
                    if((strcmp(data[i].ut_line , data[i + endCounter].ut_line) == 0 && 
                        strcmp(data[i].ut_host , data[i + endCounter].ut_host) == 0 )
                      ||
                        (strcmp(data[i].ut_line , data[i + endCounter].ut_line) == 0 && 
                         strcmp(data[i].ut_user , data[i + endCounter].ut_user) == 0))

                        {
                                session_time_info = show_time((time_t)data[i].tv_sec , (time_t)data[i + endCounter].tv_sec , 4);
        
                                // Print formatted information
                                printf("%-12s%-12s%-27s%-17s- %-8s%-30s%-30s\n",
                                    data[i].ut_user,
                                    data[i].ut_line,
                                    data[i].ut_host,
                                    session_time_info->login_time,
                                    session_time_info->logout_time,
                                    session_time_info->login_duration,
                                    record_types[data[i].ut_type]);
                                free(session_time_info);  // Free allocated memory
                                break;
                        }
                }
                endCounter++;
            }
        }
    }

    // Free memory for the data and entry structures to prevent memory leaks
    free(entry->data);
    free(entry);

    return 0;
}

