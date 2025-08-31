#ifndef UTILS_H
#define UTILS_H

#include <glibmm/ustring.h>
#include <iostream>
#include <vector>
#include <filesystem>

namespace MCSM
{
    const std::vector<Glib::ustring> list_servers(std::string path_to_folder);

    const std::string findValueInFileByParameterName(std::string parameter_name, std::string file_path);
}

#endif // UTILS_H