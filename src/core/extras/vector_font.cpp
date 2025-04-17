#include <string>
#include <sstream>
#include <iostream>

#include <vector_font.hpp>

namespace Core
{
    void VectorFont::initialize()
    {
        // Load fontData into vector<string> container
        std::vector<std::string> lines;
        std::istringstream iss(fontData_);
        std::string line;

        while (std::getline(iss, line))
            lines.push_back(line);

        configure_(lines);
    }

    void VectorFont::configure_(const std::vector<std::string> &lines)
    {
        std::cout << "lines: " << lines.size() << std::endl;
        for (auto &&line : lines)
        {
            std::cout << line << std::endl;
        }
    }

} // namespace Core
