// session_process.h
// This header declares the function for handling user session data processing.
// The session_handle function processes session records read from the utmp file
// and writes the formatted output to the log file according to the configuration.

#pragma once // Ensures this header is included only once during compilation.

#include "utmp_collector.h"
#include "datetime.h"
#include "read_config.h"

// Processes session data and writes output to the log file based on the configuration.
void session_handle(struct utmp_data *entry, struct config_data *cfg);
