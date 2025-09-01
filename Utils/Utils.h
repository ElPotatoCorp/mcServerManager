#ifndef UTILS_H
#define UTILS_H

#include <glibmm/ustring.h>
#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>

namespace MCSM
{
    const std::vector<Glib::ustring> list_servers(const std::string& path_to_folder);

    const std::string findValueInFileByParameterName(std::fstream& file, const std::string& parameter);
}

#endif // UTILS_H