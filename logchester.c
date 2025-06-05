#include "session_process.h"
#include "send_data.h"
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
    session_handle(entry);

    send_file();
    return 0;
}

