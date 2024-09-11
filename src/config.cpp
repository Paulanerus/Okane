#include "config.hpp"

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4996)
#endif

#include "rang.hpp"

#include "utils/regex.hpp"
#include "strings.hpp"

#include <filesystem>
#include <iostream>
#include <fstream>

namespace okane
{
    void Config::load()
    {
        create_dir();

        load_file();

        load_entries();

        load_abos();

        sort_entries();
    }

    void Config::save()
    {
        create_dir();

        save_file();

        save_entries();

        save_abos();
    }

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

        config_file >> m_AppConfig.currency;
    }

    void Config::save_file()
    {
        std::ofstream config_file;
        config_file.open(m_ConfigPath);

        if (!config_file.is_open())
            return;

        config_file << m_AppConfig.currency;
    }

    void Config::save_entries()
    {
        for (auto &year : m_AppConfig.years)
        {
            auto year_dir = m_BaseDir + "/" + year.year_id;

            if (!std::filesystem::exists(year_dir))
                std::filesystem::create_directory(year_dir);

            for (auto &month : year.months)
            {
                std::ofstream monthFile{year_dir + "/" + month.month_id + ".csv", std::ios::trunc};

                if (!monthFile.is_open())
                    continue;

                for (auto &entry : month.entries)
                {
                    if (entry.type == entry::EntryType::Abo)
                        continue;

                    monthFile << entry.date << ';' << entry.tag << ';' << okane::strings::convert_to_str(entry.amount) << '\n';
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

            auto year = entry::make_year(entry_name);

            std::filesystem::directory_iterator year_iter(m_BaseDir + "/" + entry_name);

            for (auto &month : year_iter)
            {
                auto month_entry_name = month.path().filename().string();

                if (!month.is_regular_file() || !okane::rgx::matches_month(month_entry_name))
                    continue;

                std::ifstream month_file;
                month_file.open(m_BaseDir + "/" + entry_name + "/" + month_entry_name);

                if (!month_file.is_open())
                    continue;

                auto month_entry = entry::make_month(month_entry_name.substr(0, month_entry_name.size() - 4));

                std::string line;
                while (std::getline(month_file, line))
                {
                    if (!okane::rgx::matches_entry(line))
                        continue;

                    month_entry.add(entry::make_entry_from_str(line));
                }

                year.months.push_back(month_entry);
            }

            m_AppConfig.years.push_back(year);
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

            auto abo_from_str = entry::make_abo_from_str(line);

            auto abo_date = okane::strings::split_str(abo_from_str.date, '.');

            for (auto &year : m_AppConfig.years)
            {
                if (year.year_id < abo_date[2])
                    continue;

                for (auto &month : year.months)
                {
                    if (month.month_id < abo_date[1])
                        continue;

                    month.add(abo_from_str);
                }
            }

            m_AppConfig.abos.push_back(abo_from_str);
        }
    }

    void Config::save_abos()
    {
        std::ofstream abo_file{m_AboPath, std::ios::trunc};

        if (!abo_file.is_open())
            return;

        for (auto &abo : m_AppConfig.abos)
            abo_file << abo.date << ';' << abo.tag << ';' << okane::strings::convert_to_str(abo.amount) << ";" << static_cast<std::underlying_type_t<entry::PayInterval>>(abo.interval) << '\n';
    }

    void Config::sort_entries()
    {
        for (auto &year : m_AppConfig.years)
        {
            for (auto &month : year.months)
            {
                std::sort(month.entries.begin(), month.entries.end(), [](const entry::Entry &e1, const entry::Entry &e2)
                          { 
                    if(e1.type != e2.type)
                        return e1.type > e2.type;
                    else if (e1.date != e2.date)
                        return e1.date < e2.date;
                    else
                        return e1.amount > e2.amount; });
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

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif