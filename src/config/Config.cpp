#include "Config.hpp"

AppConfig Config::appConfig;

Config::Config()
{
    namespace fs = std::filesystem;

    baseDir = getDirectory();
    configPath = baseDir + "/okane.txt";

    if (!fs::exists(baseDir))
        fs::create_directories(baseDir);

    if (!fs::exists(configPath) || fs::is_empty(configPath))
        return;

    load();
}

Config::~Config()
{
    save();
}

void Config::save()
{
    namespace fs = std::filesystem;

    if (!fs::exists(baseDir))
        fs::create_directories(baseDir);

    std::ofstream configFile;

    configFile.open(configPath);

    if (configFile.is_open())
    {
        configFile << appConfig.currency;
        configFile.close();
    }

    for (const auto &year : appConfig.years)
    {
        auto yearDir = baseDir + "/" + year.yearNr;

        if (!fs::exists(yearDir))
            fs::create_directory(yearDir);

        for (const auto &month : year.months)
        {
            std::ofstream monthFile;
            monthFile.open(yearDir + "/" + month.monthNr + ".csv");

            if (!monthFile.is_open())
                continue;

            for (const auto &entry : month.entries)
                monthFile << entry.epoch << ';' << entry.tag << ';' << entry.amount << '\n';

            monthFile.close();
        }
    }
}

void Config::load()
{
    namespace fs = std::filesystem;

    std::ifstream configFile;
    configFile.open(configPath);

    if (configFile.is_open())
    {
        configFile >> appConfig.currency;

        configFile.close();
    }

    if (!fs::exists(baseDir))
        return;

    fs::directory_iterator baseDirIter(baseDir);

    for (const auto &entry : baseDirIter)
    {
        auto entryName = entry.path().filename().string();

        if (!entry.is_directory() || !Okane::matchesYear(entryName))
            continue;

        Okane::YearEntry year{entryName};

        fs::directory_iterator yearIter(baseDir + "/" + entryName);

        for (const auto &month : yearIter)
        {
            auto monthEntryName = month.path().filename().string();

            if (!month.is_regular_file() || !Okane::matchesMonth(monthEntryName))
                continue;

            std::ifstream monthFile;
            monthFile.open(baseDir + "/" + entryName + "/" + monthEntryName);

            if (!monthFile.is_open())
                continue;

            Okane::MonthEntry month{monthEntryName.substr(0, monthEntryName.size() - 4)};

            std::string line;
            while (std::getline(monthFile, line))
            {
                if (!Okane::matchesFile(line))
                    continue;

                month << Okane::SimpleEntry::fromString(line);
            }

            year << month;

            monthFile.close();
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