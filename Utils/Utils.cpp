#include "Utils.h"

namespace MCSM
{
    const std::vector<Glib::ustring> *list_directories(const std::string &path_to_directory)
    {
        if (std::filesystem::exists(path_to_directory) || !std::filesystem::is_directory(path_to_directory))
        {
            return nullptr;
        }

        std::vector<Glib::ustring> files = {};

        for (const std::filesystem::__cxx11::directory_entry &entry : std::filesystem::directory_iterator(path_to_directory))
        {
            if (entry.is_directory())
            {
                std::string file_name = entry.path().filename();
                files.push_back(Glib::ustring(file_name));
            }
        }

        return &files;
    }

    const std::string find_val_in_file_by_prop(const std::string &path_to_file, const std::string &property)
    {
        std::ifstream file(path_to_file);
        std::string line;

        while (std::getline(file, line))
        {
            if (line.find(property) != std::string::npos)
            {
                size_t pos = line.find('=');
                if (pos != std::string::npos)
                {
                    file.close();
                    return line.substr(pos + 1);
                }
            }
        }

        file.close();

        // Not found
        std::cout << property << " has not been found in the properties file (" << path_to_file << ")" << "\n";

        return "";
    }

    void rewrite_property(const std::string &path_to_file, const std::string &property, const std::string &new_value)
    {
        if (std::filesystem::exists(path_to_file) || !std::filesystem::is_regular_file(path_to_file))
        {
            std::cout << "Oupsie, there is a problem with the path (" << path_to_file << ")" << "\n";
            return;
        }

        std::ifstream infile(path_to_file);
        if (!infile.is_open())
            throw std::runtime_error("File is not open");

        std::string line;
        std::vector<std::string> lines;
        bool found = false;
        while (getline(infile, line))
        {
            if (line.find('=') == property.length() && line.substr(0, property.length()) == property)
            {
                line = property + "=" + new_value;
                found = true;
            }
            lines.push_back(line);
        }
        infile.close();

        if (!found)
        {
            std::cout << "The value has not been found..." << "\n";
            return;
        }

        std::ofstream outfile(path_to_file);
        if (!outfile.is_open())
            throw std::runtime_error("File is not open");

        for (const std::string &line : lines)
        {
            outfile << line << "\n";
        }

        outfile.close();
    }
}