# logchester

A modular, configurable log collector and session analyzer for Linux, written in C. `logchester` reads utmp/wtmp files, processes user session data, generates human-readable reports, and can send results to a remote server via SSH/SFTP. Supports both password and SSH key-based authentication.  
**Now also supports real-time device log collection via UNIX sockets with non-blocking multi-threaded design.**

---

## Features

- **Reads and parses utmp/wtmp**: Extracts and processes user session data (user logins, logouts, session types).
- **Session analysis**: Calculates login/logout times, durations, and session status (still logged in, crash, shutdown, etc).
- **Configurable via file**: All paths, credentials, and remote destinations are set in `logchester.conf`.
- **Remote log transfer**: Sends processed logs to a remote server using SSH (libssh2), supporting both password and SSH key-based authentication.
- **Flexible authentication**: Choose between password and public/private key authentication for secure transfers.
- **File monitoring**: Uses inotify to watch for changes in log files and trigger processing automatically.
- **Device log collection via UNIX socket**: Collects and prints device logs in real-time from a configurable UNIX domain socket (SOCK_DGRAM).
- **Non-blocking, multi-threaded design**: Uses POSIX threads (`pthread`) to handle log file monitoring and device log collection concurrently, ensuring the main process is never blocked by socket operations.
- **Modular design**: Each major functionality is separated into its own module for easy maintenance and extension.
- **Memory management**: Dynamically allocates and frees resources as needed.
- **Extensible**: Easy to add new features, output formats, or support for other platforms.

---

## Project Structure

| File/Directory         | Purpose                                                                 |
|-----------------------|-------------------------------------------------------------------------|
| utmp_collector.c/h    | Read utmp/wtmp and collect session data                                 |
| datetime.c/h          | Format and calculate session time/duration                              |
| read_config.c/h       | Parse configuration file (`logchester.conf`)                            |
| send_data.c/h         | Send logs to remote server via SSH/SFTP (password or key)               |
| session_process.c/h   | Process sessions and generate reports                                   |
| devlog_collector.c/h  | Collect and print device logs from a UNIX domain socket (SOCK_DGRAM)    |
| logchester.c          | Main entry point, event loop, thread orchestration                      |
| logchester.conf       | Configuration file (paths, credentials, authentication, socket, etc)    |

---

## Configuration Example (`logchester.conf`)

```ini
# logchester.conf file
# Path to the utmp/wtmp file to monitor (usually /var/log/wtmp)
utmp_path = /var/log/wtmp

# IP address or hostname of the remote server to send logs to
ip = <REMOTE_SERVER_IP>

# Username for authentication on the remote server
username = <REMOTE_USERNAME>

# Password for authentication (leave blank if using SSH key)
password = <REMOTE_PASSWORD>

# Path to the file on the remote server where logs will be uploaded
remote_file = /path/on/remote/server/logins.txt

# Path to the local file where processed logs will be saved
log_file = /path/to/local/logins.txt

# Path to the UNIX domain socket for device log collection (SOCK_DGRAM)
# Example: /home/user/socket_name or /dev/log
socket_path = /path/to/socket

# (Optional) Path to SSH public and private keys for key-based authentication
pub_key = /path/to/ssh/public_key
pri_key = /path/to/ssh/private_key
passphrase = <KEY_PASSPHRASE>
```

- If both password and key are set, key-based authentication will be attempted first.
- If key authentication fails, password authentication will be attempted as a fallback.

---

## Receiving Logs from /dev/log

To receive logs that are written to `/dev/log` (for example, by syslog or other system services), you have two options:

1. **Directly set `socket_path` to `/dev/log`** in your `logchester.conf`:
   ```ini
   socket_path = /dev/log
   ```
   This will make logchester listen directly on the system log socket.

2. **Create a symbolic link from `/dev/log` to your custom socket**:
   If you want to use a custom socket path (e.g., `/home/user/mylog.sock`) but still receive data sent to `/dev/log`, create a symbolic link:
   ```bash
   ln -s /home/user/mylog.sock /dev/log
   ```
   Now, any data written to `/dev/log` will be redirected to your socket, and logchester will receive it.

> **Note:** You may need appropriate permissions to create or overwrite `/dev/log`. Use `sudo` if necessary.

---

## Build & Run

1. **Dependencies:**
   - GCC
   - libssh2-dev (for SSH/SFTP support)
   - POSIX threads (usually included by default)
2. **Build:**
   ```bash
   gcc -o logchester *.c -lssh2 -lpthread
   ```
3. **Run:**
   ```bash
   ./logchester
   ```
   The program will monitor the utmp/wtmp file and the configured UNIX socket, automatically processing and sending logs on changes.

---

## How it Works

- **Multi-threaded orchestration:**  
  The main program launches two threads:
  - One for monitoring utmp/wtmp file changes and processing user sessions.
  - One for listening to the configured UNIX domain socket (SOCK_DGRAM) and printing device logs in real-time.
- **Non-blocking log collection:**  
  By using threads, device log collection never blocks the main process or session analysis.
- **Session processing:**  
  When a change is detected in the utmp/wtmp file, all session records are read and processed, and a human-readable report is generated.
- **Remote transfer:**  
  The report is sent to the configured remote server via SSH/SFTP, using either password or key-based authentication.

---

## Extending & Contributing

- Modular codebase makes it easy to add new features (e.g., support for BSD, new output formats, CLI tools, or authentication methods).
- To contribute, fork the repo, make your changes, and submit a pull request.
- Please open issues for bugs or feature requests.

---

## License

This project is licensed under the GNU General Public License v3.0 (GPLv3).

Copyright (C) 2025  Alireza Masih

See the LICENSE file for details.

---

## Author

Alireza Masih

For contact: https://github.com/AlirezaMasih

---

## Acknowledgements

- Inspired by classic Unix tools like `last` and `syslog-ng`.
- Uses [libssh2](https://www.libssh2.org/) for SSH/SFTP transfers.
- Device log collection design inspired by best practices for non-blocking UNIX socket programming.
