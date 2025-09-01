#ifndef UTILS_H
#define UTILS_H

#include <glibmm/ustring.h>
#include <iostream>
#include <vector>
#include <filesystem>

namespace MCSM
{
    const std::vector<Glib::ustring> list_servers(std::string path_to_folder);

    // TODO: Rename all parameters by 'properties'
    const std::string findValueInFileByParameterName(std::string parameter_name, std::string file_path);

    // TODO: Rename all parameters by 'properties'
    void rewrite_parameter(const std::string& file_path, const std::string& parameter, const std::string& new_value);
}

#endif // UTILS_H