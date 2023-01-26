#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

int main()
{
    std::string path = "../SpaceInvaders/data/levels/";
    std::vector<std::string> levels;
    for (const auto& entry : fs::directory_iterator(path))
        levels.push_back(entry.path().string());

    for (auto& level : levels)
    {
        for (wchar_t ch : level)
        {
            std::cout << uint32_t(ch) << " ";
        }
        std::cout << std::endl;
    }
}