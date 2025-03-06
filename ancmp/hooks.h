#pragma once

void add_custom_hook(char *name, void *addr);

void *get_hooked_symbol(char *name);