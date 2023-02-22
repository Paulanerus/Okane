#pragma once

#include <vector>
#include <string>
#include <memory>

class Option
{
public:
    static std::unique_ptr<Option> find(const std::string &arg);

    static void toLowerStr(std::string &str);

    static std::vector<std::string> copyAfter(int argc, char **args);

    virtual void execute(const std::vector<std::string> &args) = 0;

private:
    static const std::vector<std::string> args;
};