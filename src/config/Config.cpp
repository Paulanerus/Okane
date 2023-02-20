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