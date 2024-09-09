#pragma once

#include "../entry/Entry.hpp"

#include <string>
#include <vector>

struct AppConfig
{
    std::string currency;
    std::vector<shared_year> years;
    std::vector<shared_abo> abos;
};

class Config
{
public:
    Config() : m_BaseDir(directory()), m_ConfigPath(m_BaseDir + "/okane.txt"), m_AboPath(m_BaseDir + "/abos.csv")
    {
        create_dir();

        load_file();
        load_entries();
        load_abos();

        sort_entries();
    }

    ~Config()
    {
        create_dir();

        save_file();
        save_entries();
        save_abos();
    }

public:
    inline static AppConfig s_AppConfig{"€", {}, {}};

private:
    const std::string m_BaseDir;

    const std::string m_ConfigPath;

    const std::string m_AboPath;

    std::string directory();

    void load_file();

    void save_file();

    void load_entries();

    void save_entries();

    void load_abos();

    void save_abos();

    void sort_entries();

    void create_dir();
};