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
		cout << "Usage: " << argv[0] << " <" << argv[1] << ">\n";
		return 1;
	}

	// Parse arguments
	cout << "Program name: " << argv[0] << "\n";
	cout << "Arguments:\n";
	for (int i = 1; i < argc; ++i)
		cout << "  " << i << ": " << argv[i] << "\n";

	map<string, CustomSheet> sheets;

	CustomSheet test("DEFAULT", "DEFAULT NAME");
	sheets.insert({ test.getSheetName(), test });

	string input;
	do {
		SetColor(15);
		getline(cin, input);

		cout << HandleInput(input, sheets);
	} while (input != "EXIT");

	SetColor(9);
	return 0;
}