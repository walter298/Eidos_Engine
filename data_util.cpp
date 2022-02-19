#include "data_util.h"

std::string ed_stringSection(std::string s, size_t start, size_t end)
{
	std::string section;

	for (size_t i = start; i <= end; i++) {
		section.push_back(s[i]);
	}

	return section;
}

std::vector<int> ed_parseNums(std::string input)
{
	std::vector<int> nums;

	int lastUnderscoreIndex = -1;

	for (int charIndex = 0; charIndex <= input.size(); charIndex++) {
		if (input[charIndex] == '_' || charIndex == input.size()) {
			std::string currentButtonString;

			for (int numIndex = lastUnderscoreIndex + 1; numIndex < charIndex; numIndex++) {
				currentButtonString.push_back(input[numIndex]);
			}

			nums.push_back(std::stoi(currentButtonString));

			lastUnderscoreIndex = charIndex;
		}
	}

	return nums;
}