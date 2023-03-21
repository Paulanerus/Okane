#include "Config.hpp"

#include "rang.hpp"

#include "../utils/RegexUtils.hpp"
#include "../utils/StringUtils.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

AppConfig Config::appConfig;

namespace fs = std::filesystem;

Config::Config()
{
    baseDir = getDirectory();
    configPath = baseDir + "/okane.txt";
    aboPath = baseDir + "/abos.csv";

    checkAndCreateDir();

    loadFile();
    loadEntries();
    loadAbos();

    sortEntries();
}

Config::~Config()
{
    checkAndCreateDir();

    saveFile();
    saveEntries();
    saveAbos();
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
            {
                if (entry->getType() == EntryType::ABO)
                    continue;

                monthFile << entry->getDate() << ';' << entry->getTag() << ';' << std::fixed << std::setprecision(2) << entry->getAmount() << '\n';
            }
        }
    }
}

void Config::loadEntries()
{
    fs::directory_iterator baseDirIter(baseDir);

    for (const auto &entry : baseDirIter)
    {
        auto entryName = entry.path().filename().string();

        if (!entry.is_directory() || !Okane::Regex::matchesPNumber(entryName))
            continue;

        auto year = Entry::make_year(entryName);

        fs::directory_iterator yearIter(baseDir + "/" + entryName);

        for (const auto &month : yearIter)
        {
            auto monthEntryName = month.path().filename().string();

            if (!month.is_regular_file() || !Okane::Regex::matchesMonth(monthEntryName))
                continue;

            std::ifstream monthFile;
            monthFile.open(baseDir + "/" + entryName + "/" + monthEntryName);

            if (!monthFile.is_open())
                continue;

            auto monthEntry = Entry::make_month(monthEntryName.substr(0, monthEntryName.size() - 4));

            std::string line;
            while (std::getline(monthFile, line))
            {
                if (!Okane::Regex::matchesEntry(line))
                    continue;

                monthEntry->add(Entry::fromString(line));
            }

            year->add(monthEntry);
        }

        appConfig.years.push_back(year);
    }
}

void Config::loadAbos()
{
    if (!fs::exists(aboPath) || fs::is_empty(aboPath))
        return;

    std::ifstream aboFile{aboPath};

    if (!aboFile.is_open())
        return;

    std::string line;
    while (std::getline(aboFile, line))
    {
        if (!Okane::Regex::matchesAbo(line))
            continue;

        auto aboFromStr = Entry::fromStringAbo(line);

        auto aboDate = Okane::String::splitStr(aboFromStr->getDate(), '.');

        for (const auto &year : appConfig.years)
        {
            if (year->yearNr < aboDate[2])
                continue;

            for (const auto &month : year->months)
            {
                if (month->monthNr < aboDate[1])
                    continue;

                month->add(aboFromStr);
            }
        }

        appConfig.abos.push_back(aboFromStr);
    }
}

void Config::saveAbos()
{
    std::ofstream aboFile;
    aboFile.open(aboPath);

    if (!aboFile.is_open())
        return;

    for (const auto &abo : appConfig.abos)
        aboFile << abo->getDate() << ';' << abo->getTag() << ';' << std::fixed << std::setprecision(2) << abo->getAmount() << ";" << abo->getInterval() << '\n';
}

void Config::sortEntries()
{
    for (const auto &year : appConfig.years)
    {
        for (const auto &month : year->months)
        {
            std::sort(month->entries.begin(), month->entries.end(), [](const shared_simple &e1, const shared_simple &e2)
                      { 
                    if(e1->getType() > e2->getType())
                        return true;
                    else if (e1->getType() < e2->getType())
                        return false;

                    return e1->getDate() < e2->getDate(); });
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