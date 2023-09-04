#include <iostream>
#include "morse.h"

int main()
{
	Morse morse;

	std::string example;
	std::cout << "Enter phrase to convert to the Morse code: ";
	std::getline(std::cin, example);
	std::string text = morse.strToMorse(example);
	std::cout << "\"" << example << "\": " << text << std::endl;
	morse.strToMorseSound(example);
}
