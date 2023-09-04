#include <iostream>
#include "morse.h"

int main()
{
	Morse morse;

	std::string text = morse.strToMorse("sos");
	std::cout << text << std::endl;
	morse.strToMorseSound("sos");
}
