#include <stdio.h> // For I/O operations (input/output)
#include <stdlib.h> // For memory management (malloc, realloc, free)
#include <utmp.h> // For reading utmp files
#include <time.h> // For making time field human readable
#include <string.h>


#define Empty 0
#define RunLevel 1
#define BootTime 2
#define NewTime 3
#define OldTime 4
#define InitProcess 5
#define LoginProcess 6 
#define UserProcess 7
#define DeadProcess 8


// Define the structure utmp_data to store the list of entries and its length
struct utmp_data
{
    struct utmp *data; // Array to store the utmp structures
    int length; // Number of entries in the array
};

// Function declaration to read the utmp file and return the data in a utmp_data structure
struct utmp_data *read_file(char *path);
void show_time(time_t *time , char *time_buffer_ptr , size_t size);


int main()
{
    // Read the data from the utmp file and store it in the structure
    struct utmp_data *entry = read_file("/var/log/wtmp");

    char time_buffer[17]; 
    char record_type[14];

    // Loop to print the information of each entry in the data
    for(int i = 0; i < entry->length; i++)
    {

        show_time(((time_t *)(&entry->data[i].ut_tv.tv_sec)) , time_buffer , sizeof(time_buffer));


        switch(entry->data[i].ut_type)
        {
            case Empty:
                strcpy(record_type , "\0");
                break;
            case RunLevel:
                strcpy(record_type , "Run-Level");
                break;
            case BootTime:
                strcpy(record_type , "Boot-Time");
                break;
            case NewTime:
                strcpy(record_type , "New-Time");
                break;
            case OldTime:
                strcpy(record_type , "Old-Time");
                break;
            case InitProcess:
                strcpy(record_type , "Init-Process");
                break;
            case LoginProcess:
                strcpy(record_type , "Login-Process");
                break;
            case UserProcess:
                strcpy(record_type , "User-Process");
                break;
            case DeadProcess:
                strcpy(record_type , "Dead-Process");
                break;
            
        }

            
        
        printf("%-10s%-8s%-25s%-25s%-30s\n" , entry->data[i].ut_user , entry->data[i].ut_line , entry->data[i].ut_host , time_buffer , record_type);
        
    }

    // End the use of the utmp file
    endutent();

    // Freeing memory to prevent memory leaks.
    free(entry->data);
    free(entry);

    return 0;
}
    

// Function to read the utmp file and return a structure containing the data and its length
struct utmp_data *read_file(char *path)
{
    // Allocate memory for the utmp_data structure to store the data and its length
    struct utmp_data *Returning_Data = malloc(sizeof(struct utmp_data)); 
    struct utmp *utmp_entry = NULL; // Pointer to hold each individual utmp entry
    struct utmp *entry_list = NULL; // Array to store all the entries read from the file

    int count = 0; // Keep track of the number of entries

    // Open the utmp file and prepare to read from it
    utmpname(path);  // Open the wtmp file from /var/log as a read-only file
    setutent(); // Start reading the data from the utmp file


    // Loop to read each entry from the file until the end
    while((utmp_entry = getutent()) != NULL)
    {
        // Reallocate memory to accommodate the next entry in the list
        entry_list = realloc(entry_list, (count + 1) * sizeof(struct utmp));
        if(entry_list == NULL)
        {
            // If realloc fails, free the previously allocated memory and print an error
            free(entry_list);
            free(Returning_Data);
            perror("Memory allocation failed.");
            endutent();
            return NULL;
        }    

        // Store the current utmp entry into the list
        entry_list[count++] = *utmp_entry;
    }

    // Assign the list of entries and the total number of entries to the structure to be returned
    Returning_Data->data = entry_list;
    Returning_Data->length = count;

    // Return the structure containing the list of data and the count of entries
    return Returning_Data;
}

void show_time(time_t *time , char *time_buffer_ptr , size_t size)
{


    struct tm *time_info = localtime(time);
    strftime(time_buffer_ptr , size , "%a %b %e %H:%M" , time_info);
    

}
