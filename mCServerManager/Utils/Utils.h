#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_PATH_LEN 1024
#define MAX_STR_LEN 128
#define MAX_LIST_LEN 64

size_t PTR_COUNTER = 0;

/* --- Memory Management --- */
void *_mcsm_malloc(size_t __s, const char *file, int line, const char *func);
#define mcsm_malloc(X) _mcsm_malloc( X, __FILE__, __LINE__, __FUNCTION__)

void *_mcsm_calloc(size_t __nmemb, size_t __size, const char *file, int line, const char *func);
#define mcsm_calloc(X, Y) _mcsm_calloc(X, Y, __FILE__, __LINE__, __FUNCTION__)

void *_mcsm_realloc(void *__ptr, size_t __size, const char *name, const char *file, int line, const char *func);
#define mcsm_realloc(X, Y) _mcsm_realloc(X, Y, #X, __FILE__, __LINE__, __FUNCTION__)

void *_mcsm_g_object_new(void *ptr);
#define mcsm_g_object_new(X) _mcsm_g_object_new(X)

void _mcsm_free(void *ptr, const char *name, const char *file, int line, const char *func);
#define mcsm_free(X) _mcsm_free( X, #X, __FILE__, __LINE__, __FUNCTION__)

void _mcsm_g_object_unref(void *ptr);
#define mcsm_g_object_unref(X) _mcsm_g_object_unref(X)

#define mcsm_g_clear_object(ptr) \
    do { \
        if (*(ptr) != NULL) { \
            PTR_COUNTER -= 1; \
            g_clear_object((GObject **)(ptr)); \
        } \
    } while (0)

void _mcsm_g_free(void *ptr);
#define mcsm_g_free(X) _mcsm_g_free(X)

void ptr_remaining(void);

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

/* --- String Handling --- */
char *strset(const char *__restrict__ __src);
char *strrst(char **__restrict__ __dest, const char *__restrict__ __src);
const int is_str_empty(const char *str);
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
const char *get_value_from_properties_file(const char *path, const char *property);
const int overwrite_property_from_properties_file(const char *path, const char *property, const char *new_value);
void easy_zip_from_path(const char *from, const char *entry_name, const char *to);
void easy_unzip_from_path(const char *from, const char *to);
const int create_config_directory(void);
const int create_server_config_file(const char *server_name);

#endif // UTILS_H