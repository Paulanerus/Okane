#include "Config.hpp"

#include "rang.hpp"

#include "../utils/regex.hpp"
#include "../utils/strings.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>

void Config::create_dir()
{
    if (!std::filesystem::exists(m_BaseDir))
        std::filesystem::create_directories(m_BaseDir);
}

void Config::load_file()
{
    if (!std::filesystem::exists(m_ConfigPath) || std::filesystem::is_empty(m_ConfigPath))
        return;

    std::ifstream config_file;
    config_file.open(m_ConfigPath);

    if (!config_file.is_open())
        return;

    config_file >> s_AppConfig.currency;
}

void Config::save_file()
{
    std::ofstream config_file;
    config_file.open(m_ConfigPath);

    if (!config_file.is_open())
        return;

    config_file << s_AppConfig.currency;
}

void Config::save_entries()
{
    for (const auto &year : s_AppConfig.years)
    {
        auto year_dir = m_BaseDir + "/" + year->yearNr;

        if (!std::filesystem::exists(year_dir))
            std::filesystem::create_directory(year_dir);

        for (const auto &month : year->months)
        {
            std::ofstream monthFile;
            monthFile.open(year_dir + "/" + month->monthNr + ".csv");

            if (!monthFile.is_open())
                continue;

            for (const auto &entry : month->entries)
            {
                if (entry->getType() == EntryType::ABO)
                    continue;

                monthFile << entry->getDate() << ';' << entry->getTag() << ';' << okane::strings::to_string(entry->getAmount()) << '\n';
            }
        }
    }
}

void Config::load_entries()
{
    std::filesystem::directory_iterator base_dir_iter(m_BaseDir);

    for (const auto &entry : base_dir_iter)
    {
        auto entry_name = entry.path().filename().string();

        if (!entry.is_directory() || !okane::rgx::matches_pnumber(entry_name))
            continue;

        auto year = Entry::make_year(entry_name);

        std::filesystem::directory_iterator year_iter(m_BaseDir + "/" + entry_name);

        for (const auto &month : year_iter)
        {
            auto month_entry_name = month.path().filename().string();

            if (!month.is_regular_file() || !okane::rgx::matches_month(month_entry_name))
                continue;

            std::ifstream month_file;
            month_file.open(m_BaseDir + "/" + entry_name + "/" + month_entry_name);

            if (!month_file.is_open())
                continue;

            auto month_entry = Entry::make_month(month_entry_name.substr(0, month_entry_name.size() - 4));

            std::string line;
            while (std::getline(month_file, line))
            {
                if (!okane::rgx::matches_entry(line))
                    continue;

                month_entry->add(Entry::fromString(line));
            }

            year->add(month_entry);
        }

        s_AppConfig.years.push_back(year);
    }
}

void Config::load_abos()
{
    if (!std::filesystem::exists(m_AboPath) || std::filesystem::is_empty(m_AboPath))
        return;

    std::ifstream abo_file{m_AboPath};

    if (!abo_file.is_open())
        return;

    std::string line;
    while (std::getline(abo_file, line))
    {
        if (!okane::rgx::matches_abo(line))
            continue;

        auto abo_from_str = Entry::fromStringAbo(line);

        auto abo_date = okane::strings::split_str(abo_from_str->getDate(), '.');

        for (const auto &year : s_AppConfig.years)
        {
            if (year->yearNr < abo_date[2])
                continue;

            for (const auto &month : year->months)
            {
                if (month->monthNr < abo_date[1])
                    continue;

                month->add(abo_from_str);
            }
        }

        s_AppConfig.abos.push_back(abo_from_str);
    }
}

void Config::save_abos()
{
    std::ofstream abo_file;
    abo_file.open(m_AboPath);

    if (!abo_file.is_open())
        return;

    for (const auto &abo : s_AppConfig.abos)
        abo_file << abo->getDate() << ';' << abo->getTag() << ';' << okane::strings::to_string(abo->getAmount()) << ";" << abo->getInterval() << '\n';
}

void Config::sort_entries()
{
    for (const auto &year : s_AppConfig.years)
    {
        for (const auto &month : year->months)
        {
            std::sort(month->entries.begin(), month->entries.end(), [](const shared_simple &e1, const shared_simple &e2)
                      { 
                    if(e1->getType() != e2->getType())
                        return e1->getType() > e2->getType();
                    else if (e1->getDate() != e2->getDate())
                        return e1->getDate() < e2->getDate();
                    else
                        return e1->getAmount() > e2->getAmount(); });
        }
    }
}

std::string Config::directory()
{
    std::string os_path;
#ifdef _WIN32
    os_path = std::getenv("APPDATA");
#elif __unix__
    os_path = std::getenv("HOME");
    os_path += "/.config";
#endif

    return os_path + "/Okane";
}