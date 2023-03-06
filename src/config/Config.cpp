#include "Config.hpp"

#include "rang.hpp"

#include "../regex/RegexHelper.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

AppConfig Config::appConfig;

namespace fs = std::filesystem;

Config::Config()
{
    baseDir = getDirectory();
    configPath = baseDir + "/okane.txt";

    checkAndCreateDir();

    loadFile();
    loadEntries();
}

Config::~Config()
{
    checkAndCreateDir();

    saveFile();
    saveEntries();
}

void Config::checkAndCreateDir()
{
    if (!fs::exists(baseDir))
        fs::create_directories(baseDir);
}

void Config::loadFile()
{
    if (!fs::exists(configPath) || fs::is_empty(configPath))
        return;

    std::ifstream configFile;
    configFile.open(configPath);

    if (!configFile.is_open())
        return;

    configFile >> appConfig.currency;
}

void Config::saveFile()
{
    std::ofstream configFile;
    configFile.open(configPath);

    if (!configFile.is_open())
        return;

    configFile << appConfig.currency;
}

void Config::saveEntries()
{
    for (const auto &year : appConfig.years)
    {
        auto yearDir = baseDir + "/" + year->yearNr;

        if (!fs::exists(yearDir))
            fs::create_directory(yearDir);

        for (const auto &month : year->months)
        {
            std::ofstream monthFile;
            monthFile.open(yearDir + "/" + month->monthNr + ".csv");

            if (!monthFile.is_open())
                continue;

            for (const auto &entry : month->entries)
                monthFile << entry->getDate() << ';' << entry->getTag() << ';' << std::fixed << std::setprecision(2) << entry->getAmount() << '\n';
        }
    }
}

void Config::loadEntries()
{
    fs::directory_iterator baseDirIter(baseDir);

    for (const auto &entry : baseDirIter)
    {
        auto entryName = entry.path().filename().string();

        if (!entry.is_directory() || !Okane::matchesYear(entryName))
            continue;

        auto year = Entry::make_year(entryName);

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

            auto monthEntry = Entry::make_month(monthEntryName.substr(0, monthEntryName.size() - 4));

            std::string line;
            while (std::getline(monthFile, line))
            {
                if (!Okane::matchesEntry(line))
                    continue;

                monthEntry->add(SimpleEntry::fromString(line));
            }

            year->add(monthEntry);
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