#include "CustomSheet.h"

#include <iostream>

CustomSheet::CustomSheet(string sName, string cName) {
	sheetName = sName;
	characterName = cName;
};

// Create a new custom field
void CustomSheet::addCustomField(const string& fieldName, const string& fieldValue) {
	customFields.insert({ fieldName, fieldValue });
	cout << "\nADDED FIELD " + fieldName + " WITH VALUE " + fieldValue + " TO CHARACTER " + characterName + "\n";
}; 

// Remove a custom field
void CustomSheet::removeCustomField(const string& fieldName) {
	customFields.erase(fieldName);
	cout << "\nREMOVED FIELD " + fieldName + " FROM CHARACTER " + characterName + "\n";
}; 

// Retrieve the value of a custom field
string CustomSheet::getCustomField(const string& fieldName) {
	try {
		auto value = customFields.find(fieldName);
		if (value == customFields.end())
			throw string("ERROR CODE -1: FIELD DOES NOT EXIST\n");

		if (characterName.back() == 's' || characterName.back() == 'S' || characterName.back() == 'z' || characterName.back() == 'Z')
			return "\n" + characterName + "' " + fieldName + ": " + customFields[fieldName];
		else
			return "\n" + characterName + "'s " + fieldName + ": " + customFields[fieldName];
	}
	catch (const string& error) {
		return "\n" + error;
	}
}; 

// Update the value of a custom field
string CustomSheet::updateCustomField(const string& type, const string& fieldName, const string& newValue) {
	try {
		auto value = customFields.find(fieldName);
		if (value == customFields.end())
			throw string("ERROR CODE -1: FIELD DOES NOT EXIST\n");

		string copiedValue = customFields[fieldName];

		if (type == "FIELD") {
			removeCustomField(fieldName);
			addCustomField(newValue, copiedValue);
			return "\CHANGED NAME OF FIELD " + fieldName + " TO " + newValue + "\n";
		}
		else if (type == "VALUE") {
			customFields[fieldName] = newValue;
			return "\CHANGED VALUE IN FIELD " + fieldName + " FROM " + copiedValue + " TO " + newValue + "\n";
		}
		else
			throw string("ERROR CODE -2: EXPECTED field OR value FOR UPDATE TYPE\n");
	}
	catch (const string& error) {
		return "\n" + error;
	}
}

/* Destroy relevant data
 * (Shuts up the compiler) */
CustomSheet::~CustomSheet()
{
	
};