#pragma once

#include "entry.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace okane {
struct AppConfig {
    std::string currency;
    std::vector<entry::Year> years;
    std::vector<entry::Entry> abos;
};

class Config {
public:
    Config()
        : m_BaseDir(directory())
        , m_ConfigPath(m_BaseDir + "/okane.txt")
        , m_AboPath(m_BaseDir + "/abos.csv")
    {
    }

    void load();

    void save();

    AppConfig& app_config() noexcept
    {
        return m_AppConfig;
    }

private:
    const std::string m_BaseDir;

    const std::string m_ConfigPath;

    const std::string m_AboPath;

    AppConfig m_AppConfig { "€", {}, {} };

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

namespace internal {
    inline Config& global_config()
    {
        static Config config;

        return config;
    }
}

inline AppConfig& app_config() noexcept
{
    return internal::global_config().app_config();
}

inline void load_config()
{
    internal::global_config().load();
}

inline void save_config()
{
    internal::global_config().save();
}
}
