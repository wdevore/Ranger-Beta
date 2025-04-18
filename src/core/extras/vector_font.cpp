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

        // for (const auto &keyValue : charMap)
        // {
        //     const std::string &name = keyValue.first;
        //     const CharVectors &charV = keyValue.second;
        //     // std::cout << name << std::endl;
        //     // std::cout << charV.width << " x " << charV.height << std::endl;
        //     std::cout << charV << std::endl;
        // }
    }

    void VectorFont::configure_(const std::vector<std::string> &lines)
    {
        std::cout << "lines: " << lines.size() << std::endl;
        using regIter = std::regex_iterator<std::string::const_iterator>;

        std::string mapChar{};
        int index{};
        std::string matchData{};
        CharVectors cv;

        while (true)
        {
            // ===========================
            // Try to read a char label
            // ===========================
            const std::string &label = lines.at(index++);
            // std::cout << "label: '" << label << "'" << std::endl;
            std::smatch match; // For std::string input

            if (label == "EOF")
            {
                // std::cout << "EOF detected" << std::endl;
                break;
            }

            // Attempt to match a single character
            if (std::regex_match(label, match, labelExpr))
            {
                mapChar = match[0];
                cv = CharVectors{};
                cv.setLabel(mapChar);
                // std::cout << "'" << label << "' matches the entire pattern." << std::endl;
            }
            else
            {
                std::cout << "NO label match" << std::endl;
            }

            // ===========================
            // Try to read vertices until EOC
            // ===========================
            matchData = lines.at(index++);
            // std::cout << "matchData: " << matchData << std::endl;

            while (matchData != "EOC")
            {
                regIter it(matchData.begin(), matchData.end(), verticesExpr);
                regIter end; // Default-constructed, represents the end

                // std::cout << "matches: " << it->size() << std::endl;

                cv.newPath();
                while (it != end)
                {
                    std::smatch match = *it; // Dereference the iterator to get the current match
                    // std::cout << "Found0 " << match[0] << " at position " << match.position() << std::endl;
                    // std::cout << "Found1 " << match[1] << " at position " << match.position() << std::endl;
                    // std::cout << "Found2 " << match[2] << " at position " << match.position() << std::endl;
                    ++it; // Move to the next match
                    cv.addVertex(std::stod(match[1]), std::stod(match[2]));
                }

                matchData = lines.at(index++);

                // std::cout << "Next matchData: " << matchData << std::endl;
            }

            charMap[cv.getLabel()] = cv;
        }
    }

} // namespace Core
