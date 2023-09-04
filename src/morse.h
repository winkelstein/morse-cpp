#pragma once
#include <string>
#include <vector>

constexpr int POINT_DUR = 500;
constexpr int DASH_DUR = POINT_DUR * 3;
constexpr int SPACE_DUR = 0;
constexpr int SYMBOL_SPACE_DUR = POINT_DUR / 6;

class Morse
{
public:
    std::string encode(const char symbol);
    std::vector<int> encode_str(const std::string &symbol);

public:
    std::string strToMorse(const std::string &text);
    void strToMorseSound(const std::string &text);
};