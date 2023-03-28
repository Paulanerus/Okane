#pragma once

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>

class Option
{
public:
    static std::shared_ptr<Option> find(const std::string &arg);

    static std::vector<std::string> copyAfter(int argc, char **args);

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
    static const std::unordered_map<std::unordered_set<std::string>, std::shared_ptr<Option>, HashUSet> m_Args;
};