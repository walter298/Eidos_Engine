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

void printTwo(int x, int y)
{
	std::cout << x << ", " << y << std::endl;
}

std::string ed_writeNums(std::vector<int> nums) {
	std::string s;

	for (size_t i = 0; i < nums.size(); i++) {
		s.append(std::to_string(nums[i]));

		if (i != nums.size() - 1) {
			s.append("_");
		}
	}

	std::cout << "returning " << s << std::endl;

	return s;
}