#pragma once

#include <memory>
#include <string>
#include <unordered_set>
#include <vector>

namespace okane {
class Option {
public:
    virtual const std::unordered_set<std::string> identifier() const noexcept = 0;

    virtual void execute(const std::vector<std::string>& args) = 0;
};

std::shared_ptr<Option> find_by_name(const std::string& arg) noexcept;

std::vector<std::string> copy_after(int argc, char** args) noexcept;
}
