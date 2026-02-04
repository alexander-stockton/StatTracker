#include "CustomSheet.h"
#include "functions.h"

#include <windows.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <set>

using namespace std;

int main(int argc, char* argv[]) {
	SetColor(9);

	PROGRAM_FLAGS.NERD = (argv[1] == "-n") ? 1 : 0; // Default to normal mode
	if (argc == 2) {
		std::cout << "Usage: " << argv[0] << " <" << argv[1] << ">\n";
		return 1;
	}

	std::cout << "Program name: " << argv[0] << "\n";
	std::cout << "Arguments:\n";
	for (int i = 1; i < argc; ++i) {
		std::cout << "  " << i << ": " << argv[i] << "\n";
	}

	map<string, CustomSheet> sheets;

	CustomSheet test("DEFAULT", "DEFAULT NAME");
	sheets.insert({ test.getSheetName(), test });

	// RUN TESTS
	cout << test.getCustomField("Test") << "\n";

	test.addCustomField("Test", "heheheha");
	cout << test.getCustomField("Test") << "\n";
	
	test.updateCustomField("VALUE", "Test", "hahahahe");
	cout << test.getCustomField("Test") << "\n";
	
	test.updateCustomField("FIELD", "Test", "tester");
	cout << test.getCustomField("tester") << "\n";

	test.removeCustomField("tester");
	cout << test.getCustomField("tester") << "\n";

	string input;
	do {
		SetColor(15);
		getline(cin, input);

		cout << HandleInput(input, sheets);
	} while (input != "EXIT");

	SetColor(9);
	return 0;
}