#include "send_data.h"


int send_file(struct config_data *cfg)
{
    LIBSSH2_SESSION *session;
    LIBSSH2_SFTP *sftp;
    LIBSSH2_SFTP_HANDLE *sftp_handle;

    // Initialize the libssh2 library
    libssh2_init(0);

    // Create a TCP socket for the SSH connection
    int socket_fd = socket(AF_INET , SOCK_STREAM , 0);
    struct sockaddr_in ninfo;
    int rc;

    ninfo.sin_family = AF_INET;                      // Use IPv4
    ninfo.sin_port = htons(22);                      // Default SSH port
    ninfo.sin_addr.s_addr = inet_addr(cfg->ip);      // Convert IP string to binary format

    // Connect to the remote SSH server
    connect(socket_fd , (struct sockaddr *)&ninfo , sizeof(struct sockaddr_in));

    // Initialize a new SSH session
    session = libssh2_session_init();

    // Perform SSH handshake
    libssh2_session_handshake(session , socket_fd);

    // Try public key authentication first
    rc = libssh2_userauth_publickey_fromfile(session, cfg->username, cfg->public_key, cfg->private_key, cfg->passphrase);
    if(rc != 0)
    {
        // Fallback to password authentication if key-based fails
        rc = libssh2_userauth_password(session , cfg->username , cfg->password);
        if(rc == 0 && cfg->auth_status != 0)
        {
            printf("Authentication Succeeded.\n");

            // Warn the user that password authentication is insecure
            fprintf(stderr, "[!] Warning: Password authentication is insecure. Use SSH keys instead.\n");
            cfg->auth_status = 0;
        }
        else
        {
            // If both auth methods fail, return error
            printf("Authentication Failed!\n");
            return -1;
        }
    }

    // Initialize SFTP session after successful authentication
    sftp = libssh2_sftp_init(session);

    // Open the local log file for reading
    int local_file_descriptor = open(cfg->log_file , O_RDONLY);

    // Open (or create) the file on the remote server for writing
    sftp_handle = libssh2_sftp_open(sftp , cfg->remote_file , 
                                    LIBSSH2_FXF_WRITE | LIBSSH2_FXF_CREAT , 
                                    LIBSSH2_SFTP_S_IRUSR | LIBSSH2_SFTP_S_IWUSR);  // User read/write permission

    char buffer[2048];     // Buffer for reading file chunks
    ssize_t read_file;     // Number of bytes read from local file in each iteration

    // Read from local file and write to remote file over SFTP
    while((read_file = read(local_file_descriptor , buffer , sizeof(buffer))) > 0)
    {
        libssh2_sftp_write(sftp_handle , buffer , read_file);  // Write chunk to remote file
    }

    // Close the remote SFTP file handle
    libssh2_sftp_close(sftp_handle);

    // Shutdown the SFTP session
    libssh2_sftp_shutdown(sftp);

    // Cleanly disconnect the SSH session and free resources
    libssh2_session_disconnect(session, "Shutdown");
    libssh2_session_free(session);

    // Close the local file descriptor and socket
    close(local_file_descriptor);
    close(socket_fd);

    // Finalize libssh2 library
    libssh2_exit();
    return 0;
}

