#pragma once

#include "rang.hpp"

#include "AppConfig.hpp"

#include <iostream>
#include <fstream>
#include <filesystem>

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

    void save();

    void load();
};