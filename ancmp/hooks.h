#ifndef ANCMP_HOOKS_H
#define ANCMP_HOOKS_H

void add_custom_hook(char *name, void *addr);

void *get_hooked_symbol(char *name);

#endif