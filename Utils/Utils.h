#ifndef UTILS_H
#define UTILS_H

#include <glibmm/ustring.h>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>

namespace MCSM
{
    const bool is_directory(const std::string &path_to_directory);

    const bool is_file(const std::string &path_to_file);

    const std::vector<Glib::ustring> list_directories(const std::string &path_to_directory);

    const std::vector<Glib::ustring> list_files(const std::string &path_to_directory);

    const std::string find_val_in_file_by_prop(const std::string &path_to_file, const std::string &property);

    void rewrite_property(const std::string &path_to_file, const std::string &property, const std::string &new_value);

    void easy_zip(const std::string &from, const std::string &to);
    
    void easy_unzip(const std::string &from, const std::string &to);

    void move_data(const std::string &from, const std::string &to);

    void create_config_directory();
}

#endif // UTILS_H