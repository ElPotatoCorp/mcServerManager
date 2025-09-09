#include "Utils.h"

#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

struct dirent *dir;
struct stat st = {0};

struct StringList *new_string_list(void)
{
    struct StringList *list = malloc(sizeof(struct StringList));
    if (list) {
        list->strings = malloc(MAX_LIST_LEN * sizeof(char *));
        for (int i = 0; i < MAX_LIST_LEN; i++)
        {
            list->strings[i] = malloc(MAX_STR_LEN * sizeof(char));
        }
        list->size = 0;
    }
    return list;
}

void free_string_list(struct StringList *string_list)
{
    if (string_list)
    {
        for (int i = 0; i < MAX_LIST_LEN; i++)
        {
            free(string_list->strings[i]);
        }
        free(string_list->strings);
    }
    free(string_list);
}

void append_string_list(struct StringList *string_list, const char *string)
{
    strncpy(string_list->strings[string_list->size], string, MAX_STR_LEN - 1);
    string_list->strings[string_list->size][MAX_STR_LEN - 1] = '\0';
    string_list->size++;
}

void print_string_list(const struct StringList *string_list)
{
    for (int i = 0; i < string_list->size; i++)
    {
        printf("%s\n", string_list->strings[i]);
    }
}

const int exists(const char *path)
{
    return stat(path, &st) != -1;
}

const int is_regular_file(const char *path)
{
    return exists(path) && S_ISREG(st.st_mode);
}

const int is_directory(const char *path)
{
    return exists(path) && S_ISDIR(st.st_mode);
}

struct StringList *list_entries(const char *path)
{
    if (!is_directory(path))
    {
        printf("Error with the path or the directory");
        return NULL;
    }

    struct StringList *entries = new_string_list();

    int skip_relatives = 0; // Skip '.' and '..'

    DIR *d = opendir(path);
    if (d) 
    {
        while ((dir = readdir(d)) != NULL) 
        {
            if (skip_relatives >= 2)
            {
                append_string_list(entries, dir->d_name);
            }
            else
            {
                skip_relatives++;
            }
        }
        closedir(d);
    }

    return entries;
}

struct StringList *list_directories_from_path(const char *path)
{
    if (!is_directory(path))
    {
        return NULL;
    }


    struct StringList *entries = list_entries(path);
    struct StringList *directories = new_string_list();

    char *entry_path = malloc(MAX_STR_LEN * sizeof(char));
    for (int i = 0; i < entries->size; i++)
    {
        snprintf(entry_path, MAX_STR_LEN, "%s%s", path, entries->strings[i]);
        if (is_directory(entry_path))
        {
            append_string_list(directories, entry_path);
        }
    }

    free_string_list(entries);
    free(entry_path);

    return directories;
}

struct StringList *list_regular_files_from_path(const char *path)
{
    if (!is_directory(path))
    {
        return NULL;
    }

    struct StringList *entries = list_entries(path);
    struct StringList *files = new_string_list();

    char *entry_path = malloc(MAX_STR_LEN * sizeof(char));
    for (int i = 0; i < entries->size; i++)
    {
        snprintf(entry_path, MAX_STR_LEN, "%s%s", path, entries->strings[i]);
        if (is_regular_file(entry_path))
        {
            append_string_list(files, entry_path);
        }
    }
    
    free_string_list(entries);
    free(entry_path);

    return files;
}

const char *get_value_from_properties_file_path(const char *path, const char *property)
{
    if (!is_regular_file(path))
    {
        return NULL;
    }

    FILE *file = fopen(path, "r");

    if (file == NULL)
    {
        printf("The file is not opened.\n");
        return "";
    }

    char line[MAX_STR_LEN];
    char prop[MAX_STR_LEN];
    char val[MAX_STR_LEN];

    while (fgets(line, MAX_STR_LEN, file))
    {
        if (!sscanf(line, "%[^=]=%[^\n]", prop, val))
        {
            continue;
        }
        else if (strcmp(property, prop) == 0)
        {
            fclose(file);

            char *c_val = malloc(MAX_STR_LEN * sizeof(char));
            strncpy(c_val, val, MAX_STR_LEN - 1);
            c_val[MAX_STR_LEN - 1] = '\0';
            return c_val;
        }
    }
    
    fclose(file);

    return "";
}

const int write_property_from_properties_file_path(const char *path, const char *property, const char *new_value)
{
    if (!is_regular_file(path))
    {
        printf("Error with the path or the file");
        return 0;
    }

    FILE *file = fopen(path, "r");

    struct StringList *lines = new_string_list();
    char line[MAX_STR_LEN];
    char prop[MAX_STR_LEN];
    char val[MAX_STR_LEN];

    int found = 0;
    while (fgets(line, MAX_STR_LEN, file))
    {
        if (!sscanf(line, "%[^=]=%[^\n]", prop, val))
        {
            append_string_list(lines, line);
            continue;
        }
        else if (strcmp(property, prop) == 0)
        {
            found = 1;
            sprintf(line, "%s=%s\n", prop, new_value);
        }
        append_string_list(lines, line);
    }
    
    fclose(file);

    file = fopen(path, "w");

    for (int i = 0; i < lines->size; i++)
    {
        fprintf(file, "%s", lines->strings[i]);
    }

    fclose(file);

    return found;
}