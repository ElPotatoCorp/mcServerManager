#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define MAX_STR_LEN 64
#define MAX_LIST_LEN 32

/* --- A good ol' struct for a basic list of string --- */
struct StringList 
{
    char **strings;
    int size;
};
struct StringList *new_string_list(void);
void free_string_list(struct StringList *string_list);
void append_string_list(struct StringList *string_list, const char *string);
void print_string_list(const struct StringList *string_list);

/* --- Check the path --- */
const int exists(const char *path);
const int is_regular_file(const char *path);
const int is_directory(const char *path);

/* --- Gather stuff in the computer --- */
struct StringList *list_entries(const char *path);
struct StringList *list_directories_from_path(const char *path);
struct StringList *list_regular_files_from_path(const char *path);

#endif // UTILS_H