#pragma once

#include <vector>
#include <string>
#include <map>

using namespace std;

class CustomSheet {
private:
	string sheetName;
	string characterName;

	map<string,string> customFields = {};

	string addFormat = "";
	string removeFormat = "";
	string getFormat = "";
	string updateFormat = "";

public:
	CustomSheet(string sName, string cName);

	void addCustomField(const string& fieldName, const string& fieldValue); // Create a new custom field
	void removeCustomField(const string& fieldName); // Remove a custom field
	string getCustomField(const string& fieldName); // Retrieve the value of a custom field
	string updateCustomField(const string& type, const string& fieldName, const string& newValue); // Update the value of a custom field

	~CustomSheet();
};