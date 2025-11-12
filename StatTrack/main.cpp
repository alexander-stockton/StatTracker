#include "CustomSheet.h"
#include <iostream>

using namespace std;

int main() {
	CustomSheet test("DEFAULT", "DEFAULT NAME");
	cout << test.getCustomField("Test") << "\n";

	return 0;
}