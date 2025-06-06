#include "send_data.h"

#define username "masih"                     // Username on the remote server
#define remote_file "/home/masih/log.txt"    // Path to the file on the remote server
#define file_path "log.txt"                  // Path to the local file to be sent
#define password "Alireza.md1383"            // Password for the remote user
#define hostname "127.0.0.1"                 // IP address of the remote server (localhost in this case)

void send_file()
{
    LIBSSH2_SESSION *session;
    LIBSSH2_SFTP *sftp;
    LIBSSH2_SFTP_HANDLE *sftp_handle;

    // Initialize the libssh2 library
    libssh2_init(0);

    // Create a TCP socket for the SSH connection
    int socket_fd = socket(AF_INET , SOCK_STREAM , 0);
    struct sockaddr_in ninfo;

    ninfo.sin_family = AF_INET;                     // Use IPv4
    ninfo.sin_port = htons(22);                    // SSH port
    ninfo.sin_addr.s_addr = inet_addr(hostname);   // Convert IP string to binary form

    // Connect to the remote SSH server
    connect(socket_fd , (struct sockaddr *)&ninfo , sizeof(struct sockaddr_in));

    // Initialize a new SSH session
    session = libssh2_session_init();

    // Perform SSH handshake
    libssh2_session_handshake(session , socket_fd);

    // Authenticate with username and password
    libssh2_userauth_password(session , username , password);

    // Initialize SFTP session
    sftp = libssh2_sftp_init(session);

    // Open the local file for reading
    int local_file_descriptor = open(file_path , O_RDONLY);

    // Open (or create) the file on the remote server for writing
    sftp_handle = libssh2_sftp_open(sftp , remote_file , 
                                    LIBSSH2_FXF_WRITE | LIBSSH2_FXF_CREAT , 
                                    LIBSSH2_SFTP_S_IRUSR | LIBSSH2_SFTP_S_IWUSR);

    char buffer[2048];   // Buffer to store chunks of file data
    ssize_t read_file;   // Number of bytes read per iteration

    // Read the local file and write its contents to the remote file
    while((read_file = read(local_file_descriptor , buffer , sizeof(buffer))) > 0)
    {
        libssh2_sftp_write(sftp_handle , buffer , read_file);
    }

    // Close the remote file handle
    libssh2_sftp_close(sftp_handle);

    // Shutdown the SFTP session
    libssh2_sftp_shutdown(sftp);

    // Disconnect the SSH session and free its memory
    libssh2_session_disconnect(session, "Shutdown");
    libssh2_session_free(session);

    // Close the local file and TCP socket
    close(local_file_descriptor);
    close(socket_fd);

    // Exit libssh2 library
    libssh2_exit();
}

