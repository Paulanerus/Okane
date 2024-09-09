#pragma once

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <memory>

class Option
{
public:
    static std::shared_ptr<Option> find(const std::string &arg);

    static std::vector<std::string> copy_after(int argc, char **args);

    virtual void execute(const std::vector<std::string> &args) = 0;

protected:
    struct HashUSet
    {
        std::size_t operator()(const std::unordered_set<std::string> &set) const
        {
            std::size_t hash{};
            for (const auto &elem : set)
                hash ^= std::hash<std::string>{}(elem);

            return hash;
        }
    };

private:
    static const std::unordered_map<std::unordered_set<std::string>, std::shared_ptr<Option>, HashUSet> s_Args;
};