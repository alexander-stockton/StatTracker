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

int main() {
	setColor(9);

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
		setColor(15);
		getline(cin, input);

		cout << HandleInput(input, sheets);
	} while (input != "EXIT");

	setColor(9);
	return 0;
}