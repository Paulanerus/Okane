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
    std::fstream configFile;
    configFile.open(configPath, std::ios::in);

    if (configFile.is_open())
    {
        configFile >> appConfig.currency;

        configFile.close();
    }
}

std::string Config::getDirectory()
{
#ifdef _WIN32
    // TODO
    return std::getenv("HOMEDRIVE") + std::string(std::getenv("HOMEPATH"));
#endif

#ifdef __unix__
    return std::getenv("HOME") + std::string("/.config/Okane");
#endif

#ifdef __APPLE__
    // TODO
    return std::getenv("HOME");
#endif
}