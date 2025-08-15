#include "../include/session_process.h"  // Include the header for session handling functions

void session_handle(struct utmp_data *entry , struct config_data *cfg)
{
    // Pointer to session data and session time information
    struct session_data *data = entry->data;
    struct session_time *session_time_info = NULL;

    // Declare variables for tracking counters and time offset
    int endCounter = 0;
    int innerCounter = 0;
    uint32_t time_offset;

    // Array to hold the types of records (for easy reference in the log)
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

    // Open log file for writing
    FILE *login_logs = fopen(cfg->login_logs, "w");

    // Loop through all the entries in the provided utmp data
    for (int i = 0; i < entry->length; i++)
    {
        // If the record type is BOOT_TIME (value 2)
        if (data[i].ut_type == 2)
        {
            endCounter = 1;

            // Loop to check entries following the current one, looking for a matching or shutdown entry
            while ((i + endCounter) <= entry->length)
            {
                // If we have reached the end of the entries
                if (i + endCounter == entry->length)
                {
                    // Call show_time with 0 (indicating no crash)
                    session_time_info = show_time((time_t)data[i].tv_sec, 0, 0);

                    // Write formatted data into the log file
                    fprintf(login_logs, "%-12s%-12s%-27s%-17s- %-10s%-12s\n",
                            data[i].ut_user,
                            data[i].ut_line,
                            data[i].ut_host,
                            session_time_info->login_time,
                            session_time_info->logout_time,
                            record_types[data[i].ut_type]);

                    // Free the allocated memory for session_time_info
                    free(session_time_info);
                    break;
                }

                // If the next entry is of type BOOT_TIME (crash)
                else if (data[i + endCounter].ut_type == 2)
                {
                    session_time_info = show_time((time_t)data[i].tv_sec, (time_t)data[i + endCounter].tv_sec, 1); // 1 means crash

                    // Write formatted data into the log file
                    fprintf(login_logs, "%-12s%-12s%-27s%-17s- %-10s%-12s\n",
                            data[i].ut_user,
                            data[i].ut_line,
                            data[i].ut_host,
                            session_time_info->login_time,
                            session_time_info->logout_time,
                            record_types[data[i].ut_type]);

                    // Free the allocated memory for session_time_info
                    free(session_time_info);
                    break;
                }

                // If the next entry indicates a shutdown process
                else if (strcmp(data[i + endCounter].ut_user, "shutdown") == 0)
                {
                    session_time_info = show_time((time_t)data[i].tv_sec, (time_t)data[i + endCounter].tv_sec, 4); // 4 means shutdown

                    // Write formatted data into the log file
                    fprintf(login_logs, "%-12s%-12s%-27s%-17s- %-10s%-12s%-30s\n",
                            data[i].ut_user,
                            data[i].ut_line,
                            data[i].ut_host,
                            session_time_info->login_time,
                            session_time_info->logout_time,
                            session_time_info->login_duration,
                            record_types[data[i].ut_type]);

                    // Free the allocated memory for session_time_info
                    free(session_time_info);
                    break;
                }

                endCounter++;
            }
        }

        // If the record type is USER_PROCESS (value 7)
        else if (data[i].ut_type == 7)
        {
            endCounter = 1;

            // Loop to handle subsequent entries and identify end of user session
            while ((i + endCounter) <= entry->length)
            {
                // If we have reached the end of the entries
                if ((i + endCounter) == entry->length)
                {
                    // Call show_time with 0 (indicating user is still logged in)
                    session_time_info = show_time((time_t)data[i].tv_sec, 0, 0);
                    fprintf(login_logs, "%-12s%-12s%-27s%-17s- %-10s%-12s%-30s\n",
                            data[i].ut_user,
                            data[i].ut_line,
                            data[i].ut_host,
                            session_time_info->login_time,
                            session_time_info->logout_time,
                            session_time_info->login_duration,
                            record_types[data[i].ut_type]);

                    // Free the allocated memory for session_time_info
                    free(session_time_info);
                    break;
                }

                // If the next entry indicates a shutdown process
                else if (strcmp(data[i + endCounter].ut_user, "shutdown") == 0)
                {
                    session_time_info = show_time((time_t)data[i].tv_sec, (time_t)data[i + endCounter].tv_sec, 2); // 2 means down

                    // Write formatted data into the log file
                    fprintf(login_logs, "%-12s%-12s%-27s%-17s- %-10s%-12s%-30s\n",
                            data[i].ut_user,
                            data[i].ut_line,
                            data[i].ut_host,
                            session_time_info->login_time,
                            session_time_info->logout_time,
                            session_time_info->login_duration,
                            record_types[data[i].ut_type]);

                    // Free the allocated memory for session_time_info
                    free(session_time_info);
                    break;
                }

                // If the next record is of type DEAD_PROCESS (value 8)
                else if (data[i + endCounter].ut_type == 8)
                {
                    // Check if the user, line, or host match
                    if ((strcmp(data[i].ut_line, data[i + endCounter].ut_line) == 0 &&
                         strcmp(data[i].ut_host, data[i + endCounter].ut_host) == 0)
                        ||
                        (strcmp(data[i].ut_line, data[i + endCounter].ut_line) == 0 &&
                         strcmp(data[i].ut_user, data[i + endCounter].ut_user) == 0))
                    {
                        session_time_info = show_time((time_t)data[i].tv_sec, (time_t)data[i + endCounter].tv_sec, 4);

                        // Write formatted data into the log file
                        fprintf(login_logs, "%-12s%-12s%-27s%-17s- %-10s%-12s%-30s\n",
                                data[i].ut_user,
                                data[i].ut_line,
                                data[i].ut_host,
                                session_time_info->login_time,
                                session_time_info->logout_time,
                                session_time_info->login_duration,
                                record_types[data[i].ut_type]);

                        // Free the allocated memory for session_time_info
                        free(session_time_info);
                        break;
                    }
                }
                endCounter++;
            }
        }
    }

    // Close the log file and free any dynamically allocated memory
    fclose(login_logs);
    utmp_collector_free(entry);        // Free memory for the entry structure
}

