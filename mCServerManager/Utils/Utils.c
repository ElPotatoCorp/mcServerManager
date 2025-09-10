#include "Utils.h"

#include <sys/stat.h>
#include <stdarg.h>
#include <dirent.h>
#include <string.h>

char cwd[MAX_PATH_LEN];
static const int set_default_prog_path(void)
{
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd() error");
        return 0;
    }
    return 1;
}

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

struct StringList *new_string_list_from_strings(const int n, ...)
{
    struct StringList *string_list = new_string_list();

    char *text;

    va_list args;

    va_start(args, n);
    for (int i = 0; i < n + 1; i++)
    {
        append_string_list(string_list, va_arg(args, const char *));
    }
    va_end(args);

    return string_list;
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

const char *concat_all_strings(const int n, ...)
{
    va_list args;

    size_t size_of_final_str = 0;
    va_start(args, n);
    for (int i = 0; i < n; i++)
    {
        size_of_final_str += strlen(va_arg(args, const char *));
    }
    va_end(args);

    char *str = malloc((size_of_final_str + 1) * sizeof(char));

    va_start(args, n);
    for (int i = 0; i < n; i++)
    {
        strcat(str, va_arg(args, const char *));
    }
    va_end(args);

    str[size_of_final_str] = '\0';

    return str;
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
        perror("Error with the path or the directory");
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
        perror("There is a problem with the path");
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
        perror("There is a problem with the path");
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
        perror("The file is not opened");
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
        perror("Error with the path or the file");
        return 2;
    }

    FILE *file = fopen(path, "r");

    if (file == NULL)
    {
        perror("There was an error loading the file");
        return 2;
    }

    struct StringList *lines = new_string_list();
    char line[MAX_STR_LEN];
    char prop[MAX_STR_LEN];
    char val[MAX_STR_LEN];

    int found = 1;
    while (fgets(line, MAX_STR_LEN, file))
    {
        if (!sscanf(line, "%[^=]=%[^\n]", prop, val))
        {
            append_string_list(lines, line);
            continue;
        }
        else if (strcmp(property, prop) == 1)
        {
            found = 0;
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

void easy_zip_from_path(const char *from, const char *entry_name, const char *to)
{
    if (!exists(from) || !exists(to))
    {
        perror("There is a problem with at least one path");
        return;
    }

    chdir(from);

    const char *command = concat_all_strings(5, "zip -r \"", to, "$(date +%Y-%m-%d-%H-%M-%S)\" \"", entry_name, "\"");

    execl("/usr/bin/sh", "sh", "-c", command, 0);

    free((char *)command);
}

void easy_unzip_from_path(const char *from, const char *to)
{
    if (!exists(from) || !exists(to))
    {
        perror("There is a problem with at least one path");
        return;
    }

    const char *command = concat_all_strings(5, "unzip \"", from, "\" -d \"", to, "\"");

    execl("/usr/bin/sh", "sh", "-c", command, 0);

    free((char *)command);
}

const int create_config_directory(void)
{
    if (cwd == NULL && set_default_prog_path())
    {
        perror("There was an error setting up the default program path");
        return 2;
    }

    chdir(cwd);

    if (is_directory("./config/"))
    {
        return 1;
    }
    
    if (mkdir("./config/", 0700) != 0)
    {
        perror("There was an error creating the config directory");
        return 2;
    }

    if (is_regular_file("./config/.config"))
    {
        return 1;
    }

    FILE *file = fopen("./config/.config", "w");

    if (file == NULL)
    {
        perror("There was an error creating the .config file");
        return 2;
    }

    fputs("server-directory=\n", file);

    fclose(file);

    return 0;
}