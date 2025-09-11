#include "Utils.h"

#include <sys/stat.h>
#include <stdarg.h>
#include <dirent.h>
#include <curl/curl.h>

char cwd[MAX_PATH_LEN];
static const int set_default_prog_path(void)
{
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        perror("getcwd() error");
        return 2;
    }
    return 0;
}

struct dirent *dir;
struct stat st = {0};

#pragma region StringList
struct StringList *new_string_list(void)
{
    struct StringList *list = malloc(sizeof(struct StringList));
    if (list)
    {
        list->strings = malloc(MAX_LIST_LEN * sizeof(char *));
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
        for (int i = 0; i < string_list->size; i++)
        {
            free(string_list->strings[i]);
        }
        free(string_list->strings);
    }
    free(string_list);
}

void append_string_list(struct StringList *string_list, const char *string)
{
    size_t data_size = (strlen(string) + 1) * sizeof(char);

    string_list->strings[string_list->size] = malloc(data_size);

    strcpy(string_list->strings[string_list->size], string);

    string_list->size++;
}

void print_string_list(const struct StringList *string_list)
{
    for (int i = 0; i < string_list->size; i++)
    {
        printf("%s\n", string_list->strings[i]);
    }
}
#pragma endregion // StringList

#pragma region CURL
struct MemoryStruct
{
    char *memory;
    size_t size;
};

static size_t write_memory_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL)
    {
        printf("error: not enough memory\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

const char *curl_from_url(const char *url)
{
    CURL *curl_handle;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_handle = curl_easy_init();
    if (curl_handle)
    {
        curl_easy_setopt(curl_handle, CURLOPT_URL, url);
        curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_memory_callback);
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
        curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

        res = curl_easy_perform(curl_handle);

        if (res != CURLE_OK)
        {
            fprintf(stderr, "error: %s\n", curl_easy_strerror(res));
            free(chunk.memory);
            return NULL;
        }
        curl_easy_cleanup(curl_handle);

        return chunk.memory;
    }
    else
    {
        free(chunk.memory);
        return NULL;
    }
}
#pragma endregion // CURL

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
    str[0] = '\0';

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

    char *entry_path = malloc(MAX_PATH_LEN * sizeof(char));
    for (int i = 0; i < entries->size; i++)
    {
        snprintf(entry_path, MAX_STR_LEN, "%s%s", path, entries->strings[i]);
        if (is_directory(entry_path))
        {
            append_string_list(directories, entries->strings[i]);
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

const int overwrite_property_from_properties_file_path(const char *path, const char *property, const char *new_value)
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

    if (cwd == NULL && set_default_prog_path())
    {
        perror("There was an error setting up the default program path");
        return;
    }

    chdir(from);

    const char *command = concat_all_strings(5, "zip -r \"", to, "$(date +%Y-%m-%d-%H-%M-%S)\" \"", entry_name, "\"");

    execl("/usr/bin/sh", "sh", "-c", command, 0);

    free((char *)command);

    chdir(cwd);
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

    if (is_directory("./config/") && is_regular_file("./config/.config"))
    {
        return 0;
    }
    else
    {
        return 2;
    }
}

const int create_server_config_file(const char *server_name)
{
    const char *server_config_path = concat_all_strings(3, "./config/", server_name, ".properties");
    if (exists(server_config_path))
    {
        free((char *)server_config_path);
        return 1;
    }
    else if (!is_directory("./config/") && create_config_directory() > 0)
    {
        perror("There was an error creating the config directory");
        free((char *)server_config_path);
        return 2;
    }

    FILE *file = fopen(server_config_path, "w");

    if (file == NULL)
    {
        perror("There was an error opening the properties file");
        free((char *)server_config_path);
        return 2;
    }

    fputs("start-script-name=\n", file);

    fclose(file);
    free((char *)server_config_path);
    return 0;
}