#include "Config.hpp"

AppConfig Config::appConfig;

Config::Config()
{
    namespace fs = std::filesystem;

    baseDir = getDirectory();
    configPath = baseDir + "/okane.txt";

    if (!fs::exists(baseDir))
    {
        fs::create_directories(baseDir);
    }

    if (!fs::exists(configPath) || fs::is_empty(configPath))
    {
        return;
    }

    load();
}

Config::~Config()
{
    save();
}

void Config::save()
{
    std::fstream configFile;

    configFile.open(configPath, std::ios::out);

    if (configFile.is_open())
    {
        configFile << appConfig.currency;
        configFile.close();
    }
}

void Config::load()
{
    namespace fs = std::filesystem;

    std::fstream configFile;
    configFile.open(configPath, std::ios::in);

    if (configFile.is_open())
    {
        configFile >> appConfig.currency;

        configFile.close();
    }

    if (!fs::exists(baseDir))
    {
        return;
    }

    fs::directory_iterator baseDirIter(baseDir);

    for (const auto &entry : baseDirIter)
    {
        auto entryName = entry.path().filename().string();

        if (!entry.is_directory() || !std::regex_match(entryName, std::regex("[0-9]+")))
        {
            continue;
        }

        auto yearNr = static_cast<uint32_t>(std::stoul(entryName));
        Okane::YearEntry year{yearNr};

        fs::directory_iterator yearIter(baseDir + "/" + entryName);

        for (const auto &month : yearIter)
        {
            auto monthEntryName = month.path().filename().string();

            if (!month.is_regular_file() || !std::regex_match(monthEntryName, std::regex("([0-9]|1[0-1]).csv")))
            {
                continue;
            }

            std::fstream monthFile;
            monthFile.open(baseDir + "/" + entryName + "/" + monthEntryName, std::ios::in);

            if (monthFile.is_open())
            {
                auto monthNr = static_cast<uint32_t>(std::stoul(monthEntryName.substr(0, monthEntryName.size() - 4)));
                Okane::MonthEntry month{monthNr};

                std::string line;
                while (std::getline(monthFile, line))
                {
                    std::cout << line << std::endl;
                    // test if line is valid
                    // split into components

                    Okane::SimpleEntry entry{0, "", 0};
                    month << entry;
                }

                year << month;

                monthFile.close();
            }
        }
    }
}

std::string Config::getDirectory()
{
#ifdef _WIN32
    std::string osPath(std::getenv("APPDATA"));
#endif

#ifdef __unix__
    std::string osPath(std::getenv("HOME"));
    osPath += "/.config";
#endif

#ifdef __APPLE__
    // TODO
    std::string osPath(std::getenv("HOME"));
#endif

    return osPath + "/Okane";
}