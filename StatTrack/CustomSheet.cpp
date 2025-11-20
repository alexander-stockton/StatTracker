#include "CustomSheet.h"
#include "functions.h"

#include <iostream>

CustomSheet::CustomSheet(string sName, string cName) {
	sheetName = sName;
	characterName = cName;
};

// Create a new custom field
void CustomSheet::addCustomField(const string& fieldName, const string& fieldValue) {
	customFields.insert({ fieldName, fieldValue });
	setColor(14); cout << "\nADDED ";
	setColor(13); cout << "FIELD ";
	setColor(9); cout << "[";
	setColor(15); cout << fieldName;
	setColor(9); cout << "] ";
	setColor(12); cout << "WITH ";
	setColor(13); cout << "VALUE ";
	setColor(9); cout << "[";
	setColor(15); cout << fieldValue;
	setColor(9); cout << "] ";
	setColor(12); cout << "TO ";
	setColor(11); cout << "CHARACTER ";
	setColor(9); cout << "[";
	setColor(15); cout << characterName;
	setColor(9); cout << "]\n";
}; 

// Remove a custom field
void CustomSheet::removeCustomField(const string& fieldName) {
	customFields.erase(fieldName);
	setColor(14); cout << "\nREMOVED ";
	setColor(13); cout << "FIELD ";
	setColor(9); cout << "[";
	setColor(15); cout << fieldName;
	setColor(9); cout << "] ";
	setColor(12); cout << "FROM ";
	setColor(11); cout << "CHARACTER ";
	setColor(9); cout << "[";
	setColor(15); cout << characterName;
	setColor(9); cout << "]\n";
}; 

// Retrieve the value of a custom field
string CustomSheet::getCustomField(const string& fieldName) {
	try {
		if (customFields.find(fieldName) == customFields.end())
			throw GetErrors(0);

		if (characterName.back() == 's' || characterName.back() == 'S' || characterName.back() == 'z' || characterName.back() == 'Z')
			return "\n" + characterName + "' " + fieldName + ": " + customFields[fieldName];
		else
			return "\n" + characterName + "'s " + fieldName + ": " + customFields[fieldName];
	}
	catch (const string& error) {
		setColor(12);
		return "\n" + error;
	}
}; 

// Update the value of a custom field
void CustomSheet::updateCustomField(const string& type, const string& fieldName, const string& newValue) {
	try {
		if (customFields.find(fieldName) == customFields.end())
			throw GetErrors(0);

		string copiedValue = customFields[fieldName];

		if (type == "FIELD") {
			customFields.erase(fieldName);
			customFields.insert({ newValue, copiedValue });
			
			setColor(14); cout << "\nCHANGED ";
			setColor(11); cout << "NAME ";
			setColor(12); cout << "OF ";
			setColor(13); cout << "FIELD ";
			setColor(9); cout << "[";
			setColor(15); cout << fieldName;
			setColor(9); cout << "] ";
			setColor(12); cout << "FOR ";
			setColor(11); cout << "CHARACTER ";
			setColor(9); cout << "[";
			setColor(15); cout << characterName;
			setColor(9); cout << "] ";
			setColor(12); cout << "TO ";
			setColor(9); cout << "[";
			setColor(15); cout << newValue;
			setColor(9); cout << "]\n";
		}
		else if (type == "VALUE") {
			customFields[fieldName] = newValue;

			setColor(14); cout << "\nCHANGED ";
			setColor(13); cout << "VALUE ";
			setColor(12); cout << "IN ";
			setColor(13); cout << "FIELD ";
			setColor(9); cout << "[";
			setColor(15); cout << fieldName;
			setColor(9); cout << "] ";
			setColor(12); cout << "FOR ";
			setColor(11); cout << "CHARACTER ";
			setColor(9); cout << "[";
			setColor(15); cout << characterName;
			setColor(9); cout << "] ";
			setColor(12); cout << "FROM ";
			setColor(9); cout << "[";
			setColor(15); cout << copiedValue;
			setColor(9); cout << "] ";
			setColor(12); cout << "TO ";
			setColor(9); cout << "[";
			setColor(15); cout << newValue;
			setColor(9); cout << "]\n";
		}
		else {
			setColor(9);
			throw GetErrors(1);
		}
	}
	catch (const string& error) {
		setColor(12);
		cout << "\n" + error + "\n";
	}
}

/* Destroy relevant data
 * (Shuts up the compiler) */
CustomSheet::~CustomSheet()
{
	
};