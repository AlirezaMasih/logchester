#pragma once 

#include "collector.h"
#include "datetime.h"
#include "read_config.h"

void session_handle(struct utmp_data *entry , struct config_data *cfg);
