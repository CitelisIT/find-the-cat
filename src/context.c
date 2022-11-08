#include "context.h"
#include <stdbool.h>

void set_color() { app_context.color = true; }

void set_follow_links() { app_context.follow_links = true; }

void set_threads(int n) { app_context.threads = n; }

void set_filter_type(logical_op op) { app_context.filter_type = op; }
