#pragma once

#include "rang.hpp"

#include "AppConfig.hpp"
#include "../entry/Entry.hpp"

#include <sstream>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <regex>

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