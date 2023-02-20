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

std::vector<std::string> split(std::string &str)
{
    std::vector<std::string> parts;

    std::stringstream stream(str);
    std::string tmp;

    while (std::getline(stream, tmp, ';'))
    {
        parts.push_back(tmp);
    }

    return parts;
}

void Config::save()
{
    namespace fs = std::filesystem;

    if (!fs::exists(baseDir))
    {
        fs::create_directories(baseDir);
    }

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

        Okane::YearEntry year{entryName};

        fs::directory_iterator yearIter(baseDir + "/" + entryName);

        for (const auto &month : yearIter)
        {
            auto monthEntryName = month.path().filename().string();

            if (!month.is_regular_file() || !std::regex_match(monthEntryName, std::regex("([1-9]|1[0-2]).csv")))
            {
                continue;
            }

            std::fstream monthFile;
            monthFile.open(baseDir + "/" + entryName + "/" + monthEntryName, std::ios::in);

            if (monthFile.is_open())
            {
                Okane::MonthEntry month{monthEntryName.substr(0, monthEntryName.size() - 4)};

                std::string line;
                while (std::getline(monthFile, line))
                {
                    if (!std::regex_match(line, std::regex("^[-+]?\\d+;[a-zA-Z0-9]*;[-+]?\\d*\\.?\\d+$")))
                    {
                        continue;
                    }

                    auto parts = split(line);

                    auto epoch = std::stol(parts[0]);
                    auto amount = std::stod(parts[2]);

                    Okane::SimpleEntry entry{epoch, parts[1], amount};
                    month << entry;
                }

                year << month;

                monthFile.close();
            }
        }

        appConfig.years.push_back(year);
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