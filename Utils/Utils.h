#ifndef UTILS_H
#define UTILS_H

#include <glibmm/ustring.h>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>

namespace MCSM
{
    const std::vector<Glib::ustring> list_directories(const std::string &path_to_directory);

    const std::string find_val_in_file_by_prop(const std::string &path_to_file, const std::string &property);

    void rewrite_property(const std::string &path_to_file, const std::string &property, const std::string &new_value);
}

#endif // UTILS_H