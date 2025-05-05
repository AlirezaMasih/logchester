#include <stdio.h> // For I/O operations (input/output)
#include <stdlib.h> // For memory management (malloc, realloc, free)
#include <fcntl.h> // For file access
#include <unistd.h> // For reading files
#include <utmp.h> // For reading utmp files


// Define the structure utmp_data to store the list of entries and its length
struct utmp_data
{
    struct utmp *data; // Array to store the utmp structures
    int length; // Number of entries in the array
};

// Function declaration to read the utmp file and return the data in a utmp_data structure
struct utmp_data *read_file(char *path);


int main()
{
    // Read the data from the utmp file and store it in the structure
    struct utmp_data *entry = read_file("/var/log/wtmp");

    // Loop to print the information of each entry in the data
    for(int i = 0; i < entry->length; i++)
    {
        // Print the details of each entry from the utmp file
        printf("Type: %d\n", entry->data[i].ut_type);
        printf("User: %s\n", entry->data[i].ut_user);
        printf("TTY : %s\n", entry->data[i].ut_line);
        printf("Host: %s\n", entry->data[i].ut_host);
        printf("PID : %d\n", entry->data[i].ut_pid);
        printf("Time: %u\n", entry->data[i].ut_tv.tv_sec);
        printf("---------------------------\n");
    }

    // End the use of the utmp file
    endutent();

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

