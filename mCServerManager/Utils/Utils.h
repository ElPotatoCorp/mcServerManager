#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH_LEN 1024
#define MAX_STR_LEN 128
#define MAX_LIST_LEN 64

/* --- A good ol' struct for a basic list of string --- */
struct StringList 
{
    char **strings;
    int size;
};
struct StringList *new_string_list(void);
struct StringList *new_string_list_from_strings(const int n, ...);
void free_string_list(struct StringList *string_list);
void append_string_list(struct StringList *string_list, const char *string);
void print_string_list(const struct StringList *string_list);
char *strset(char *__restrict__ __dest, const char *__restrict__ __src);
const char *concat_all_strings(const int n, ...);

/* --- CURL --- */
const char *curl_from_url(const char *url);

/* --- Check the path --- */
const int exists(const char *path);
const int is_regular_file(const char *path);
const int is_directory(const char *path);

/* --- Gather stuff in the computer --- */
struct StringList *list_entries(const char *path);
struct StringList *list_directories_from_path(const char *path);
struct StringList *list_regular_files_from_path(const char *path);

/* --- File handling --- */
const char *get_value_from_properties_file_path(const char *path, const char *property);
const int overwrite_property_from_properties_file_path(const char *path, const char *property, const char *new_value);
void easy_zip_from_path(const char *from, const char *entry_name, const char *to);
void easy_unzip_from_path(const char *from, const char *to);
const int create_config_directory(void);
const int create_server_config_file(const char *server_name);

#endif // UTILS_H