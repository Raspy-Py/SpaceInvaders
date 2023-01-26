#pragma once
#include <string>
#include <SFML/Graphics.hpp>

#define RETRIEVE_NAME(x) ExtractSpecificator(typeid(x).name())

inline const std::string ExtractSpecificator(const std::string& className)
{
    size_t space = className.find(' ');

    if (space >= className.size() - 1)
        return "";
    else
        return className.substr(space + 1, className.size());
}

inline sf::Vector2f ParseVector(const std::string& vecStr)
{
    size_t space = vecStr.find(' ');

    if (space == 0 || space == vecStr.size() - 1)
    {
        return sf::Vector2f(0.0f, 0.0f);
    }

    sf::Vector2f vec = {
        stof(vecStr.substr(0, space)),
        stof(vecStr.substr(space + 1, vecStr.size() - space - 1))
    };

    return vec;
}

inline std::string GetNameFromPath(const std::string& path)
{
    size_t begin = -1;
    size_t end = -1;

    for (size_t i = path.size() - 1; i >= 0; i--)
    {
        if (path[i] == '.' && end == -1)
        {
            end = i;
        }
        else if (path[i] == '/' || path[i] == '\\')
        {
            begin = i + 1;

            if (end != -1) break;
        }
    }

    if (end < begin || begin >= path.size())
    {
        return "";
    }

    return path.substr(begin, end - begin);
}