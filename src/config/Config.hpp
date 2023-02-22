#pragma once

#include "AppConfig.hpp"

class Config
{
public:
    static AppConfig appConfig;

    Config();

    ~Config();

private:
    std::string baseDir;

    std::string configPath;

    std::string getDirectory();

    void loadFile();

    void saveFile();

    void loadEntries();

    void saveEntries();

    void checkAndCreateDir();
};