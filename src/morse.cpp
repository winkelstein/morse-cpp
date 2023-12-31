#include "morse.h"
#include "beep.hpp"

#include <thread>

std::string Morse::encode(const char symbol)
{
    switch (std::tolower(symbol))
    {
    case 'a':
        return ".-";
    case 'b':
        return "-...";
    case 'c':
        return "-.-.";
    case 'd':
        return "-..";
    case 'e':
        return ".";
    case 'f':
        return "..-.";
    case 'g':
        return "--.";
    case 'h':
        return "....";
    case 'i':
        return "..";
    case 'j':
        return ".----";
    case 'k':
        return "-.-";
    case 'l':
        return ".-..";
    case 'm':
        return "--";
    case 'n':
        return "-.";
    case 'o':
        return "---";
    case 'p':
        return ".--.";
    case 'q':
        return "--.-";
    case 'r':
        return ".-.";
    case 's':
        return "...";
    case 't':
        return "-";
    case 'u':
        return "..-";
    case 'v':
        return "...-";
    case 'w':
        return ".--";
    case 'x':
        return "-..-";
    case 'y':
        return "-.--";
    case 'z':
        return "--..";
    case '1':
        return ".----";
    case '2':
        return "..---";
    case '3':
        return "...--";
    case '4':
        return "....-";
    case '5':
        return ".....";
    case '6':
        return "-....";
    case '7':
        return "--...";
    case '8':
        return "---..";
    case '9':
        return "----.";
    case '0':
        return "-----";
    case ' ':
    case ',':
    case '.':
    case '!':
    case '?':
    default:
        return "/";
    }
}

std::vector<int> Morse::encode_str(const std::string &symbol)
{
    std::vector<int> result;

    for (const auto &item : symbol)
    {
        if (item == '.')
            result.push_back(POINT_DUR);
        else if (item == '-')
            result.push_back(DASH_DUR);
        else if (item == '/')
        {
            result.push_back(SPACE_DUR);
            result.push_back(SPACE_DUR);
            result.push_back(SPACE_DUR);
        }
    }

    return result;
}

std::string Morse::strToMorse(const std::string &text)
{
    std::string result;

    for (const auto &item : text)
    {
        std::string encoded = this->encode(item) + "/";
        result.insert(result.end(), encoded.begin(), encoded.end());
    }

    return result;
}

void Morse::strToMorseSound(const std::string &text)
{
    std::vector<int> result;

    for (const auto &item : text)
    {
        std::string encoded = this->encode(item) + "/";
        std::vector<int> symbol = this->encode_str(encoded);
        result.insert(result.end(), symbol.begin(), symbol.end());
    }

    for (const auto &item : result)
    {
        if (item == 0)
            std::this_thread::sleep_for(std::chrono::milliseconds(SYMBOL_SPACE_DUR));
        else
            beep(300, item);
        std::this_thread::sleep_for(std::chrono::milliseconds(SYMBOL_SPACE_DUR));
    }
}