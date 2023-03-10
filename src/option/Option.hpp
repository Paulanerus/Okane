#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

class Option
{
public:
    static std::shared_ptr<Option> find(const std::string &arg);

    static std::vector<std::string> copyAfter(int argc, char **args);

    virtual void execute(const std::vector<std::string> &args) = 0;

private:
    static const std::unordered_map<std::string, std::shared_ptr<Option>> m_Args;
};