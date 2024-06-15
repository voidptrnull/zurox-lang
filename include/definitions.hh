#ifndef DEFINITIONS_HH
#define DEFINITIONS_HH

#include <string>
#include <array>
#include <algorithm>

constexpr std::array<std::string_view, 17> DATA_TYPES = {
    "u8", "u16", "u32", "u64", "i8", "i16", "i32", "i64",
    "f32", "f64", "u128", "i128", "f80", "f128", "char", "bool"};

constexpr std::array<std::string_view, 4> ARCH_SPECIFIC_TYPES = {
    "u128", "i128", "f80", "f128"};

constexpr std::array<std::string_view, 20> KEYWORDS = {
    "if", "elif", "else", "loop", "fn", "ret", "true", "false", "ref", "deref",
    "struct", "sync", "enum", "void", "volatile", "null", "import", "break", "continue", "match"};

class DResult {
public:
    ~DResult() = default;
    DResult() = default;
    DResult(unsigned long int index, bool found) : index(index), found(found) {}
    unsigned long int index = 0;
    bool found = false;
};

DResult find_dt(const std::string_view& x) {
    auto it = std::find(DATA_TYPES.begin(), DATA_TYPES.end(), x);
    if (it != DATA_TYPES.end()) {
        return DResult(std::distance(DATA_TYPES.begin(), it), true);
    }
    return DResult(0, false);
}

DResult find_archdt(const std::string_view& x) {
    auto it = std::find(ARCH_SPECIFIC_TYPES.begin(), ARCH_SPECIFIC_TYPES.end(), x);
    if (it != ARCH_SPECIFIC_TYPES.end()) {
        return DResult(std::distance(ARCH_SPECIFIC_TYPES.begin(), it), true);
    }
    return DResult(0, false);
}

DResult find_keyword(const std::string_view& x) {
    auto it = std::find(KEYWORDS.begin(), KEYWORDS.end(), x);
    if (it != KEYWORDS.end()) {
        return DResult(std::distance(KEYWORDS.begin(), it), true);
    }
    return DResult(0, false);
}

#endif