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
	SetColor(14); cout << "\nADDED ";
	SetColor(13); cout << "FIELD ";
	SetColor(9); cout << "[";
	SetColor(15); cout << fieldName;
	SetColor(9); cout << "] ";
	SetColor(12); cout << "WITH ";
	SetColor(13); cout << "VALUE ";
	SetColor(9); cout << "[";
	SetColor(15); cout << fieldValue;
	SetColor(9); cout << "] ";
	SetColor(12); cout << "TO ";
	SetColor(11); cout << "CHARACTER ";
	SetColor(9); cout << "[";
	SetColor(15); cout << characterName;
	SetColor(9); cout << "]\n";
}; 

// Remove a custom field
void CustomSheet::removeCustomField(const string& fieldName) {
	customFields.erase(fieldName);
	SetColor(14); cout << "\nREMOVED ";
	SetColor(13); cout << "FIELD ";
	SetColor(9); cout << "[";
	SetColor(15); cout << fieldName;
	SetColor(9); cout << "] ";
	SetColor(12); cout << "FROM ";
	SetColor(11); cout << "CHARACTER ";
	SetColor(9); cout << "[";
	SetColor(15); cout << characterName;
	SetColor(9); cout << "]\n";
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
		SetColor(12);
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
			
			SetColor(14); cout << "\nCHANGED ";
			SetColor(11); cout << "NAME ";
			SetColor(12); cout << "OF ";
			SetColor(13); cout << "FIELD ";
			SetColor(9); cout << "[";
			SetColor(15); cout << fieldName;
			SetColor(9); cout << "] ";
			SetColor(12); cout << "FOR ";
			SetColor(11); cout << "CHARACTER ";
			SetColor(9); cout << "[";
			SetColor(15); cout << characterName;
			SetColor(9); cout << "] ";
			SetColor(12); cout << "TO ";
			SetColor(9); cout << "[";
			SetColor(15); cout << newValue;
			SetColor(9); cout << "]\n";
		}
		else if (type == "VALUE") {
			customFields[fieldName] = newValue;

			SetColor(14); cout << "\nCHANGED ";
			SetColor(13); cout << "VALUE ";
			SetColor(12); cout << "IN ";
			SetColor(13); cout << "FIELD ";
			SetColor(9); cout << "[";
			SetColor(15); cout << fieldName;
			SetColor(9); cout << "] ";
			SetColor(12); cout << "FOR ";
			SetColor(11); cout << "CHARACTER ";
			SetColor(9); cout << "[";
			SetColor(15); cout << characterName;
			SetColor(9); cout << "] ";
			SetColor(12); cout << "FROM ";
			SetColor(9); cout << "[";
			SetColor(15); cout << copiedValue;
			SetColor(9); cout << "] ";
			SetColor(12); cout << "TO ";
			SetColor(9); cout << "[";
			SetColor(15); cout << newValue;
			SetColor(9); cout << "]\n";
		}
		else {
			SetColor(9);
			throw GetErrors(1);
		}
	}
	catch (const string& error) {
		SetColor(12);
		cout << "\n" + error + "\n";
	}
}

/* Destroy relevant data
 * (Shuts up the compiler) */
CustomSheet::~CustomSheet()
{
	
};