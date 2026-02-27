#pragma once

#include "CustomSheet.h"
#include "functions.h"
#include <windows.h>
#include <conio.h>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <fstream>
#include <limits>
#include <ctime>

using namespace std;

// ==================================================================================================
// =                                    CONSTANT EXPRESSIONS                                        =
// ==================================================================================================
constexpr auto CLEAR = "\x1b[2J\x1b[H";
constexpr short DOWN = 80;
constexpr short UP = 72;
constexpr short RIGHT = 77;

// ==================================================================================================
// =                                          HELPERS                                               =
// ==================================================================================================

// Set the current text and cell color
void SetColor(short color) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); }

// Convert parameter into uppercase
string ToUpper(const string& str) {
	string s = str;
	transform(s.begin(), s.end(), s.begin(), toupper);
	return s;
}

// Read in arrow key input for meny navigation
static void GetNormalModeMenuInput(char& c) {
	char* _buffer = new char[2]();
	_buffer[0] = _getch();
	_buffer[1] = _getch();
	c = _buffer[1];
	delete[] _buffer;
}

static void SetFlags(bool* flags) {
	PROGRAM_FLAGS.SELECTION = flags[0];
	PROGRAM_FLAGS.MODE = flags[1];
	PROGRAM_FLAGS.STEP = flags[2];
}

// --- Tokenizer ---
// Parses through instruction string and returns vector of tokens
vector<string> Tokenize(const string& input) {
	
	
	//vector<string> tokens;
	//string currentToken;
	//bool insideBrackets = false;

	//// Set of words to ignore completely based on flowchart X's
	//set<string> ignoredWords = {
	//    "NEW", "BUT", "EXCEPT", "AS", "EXACTLY", "A", "NAMED", "CALLED", "TITLED"
	//};

	//for (size_t i = 0; i < input.length(); ++i) {
	//    char c = input[i];

	//    if (c == '[') {
	//        if (!currentToken.empty()) {
	//            tokens.push_back(currentToken);
	//            currentToken.clear();
	//        }
	//        insideBrackets = true;
	//        currentToken += c;
	//    }
	//    else if (c == ']') {
	//        currentToken += c;
	//        insideBrackets = false;
	//        tokens.push_back(currentToken);
	//        currentToken.clear();
	//    }
	//    else if (isspace(c) && !insideBrackets) {
	//        if (!currentToken.empty()) {
	//            tokens.push_back(currentToken);
	//            currentToken.clear();
	//        }
	//    }
	//    else {
	//        // Handle period at end of sentence as distinct token if needed, 
	//        // or just treat as part of word.
	//        if (c == '.' && !insideBrackets) {
	//            if (!currentToken.empty()) {
	//                tokens.push_back(currentToken);
	//                currentToken.clear();
	//            }
	//            tokens.push_back(".");
	//        }
	//        else {
	//            currentToken += c;
	//        }
	//    }
	//}
	//if (!currentToken.empty()) tokens.push_back(currentToken);

	//// Filter phase
	//vector<string> filteredTokens;
	//for (const auto& t : tokens) {
	//    // Keep bracketed variables
	//    if (t.front() == '[' && t.back() == ']') {
	//        filteredTokens.push_back(t);
	//    }
	//    else {
	//        // It's a keyword
	//        if (ignoredWords.find(t) == ignoredWords.end()) {
	//            filteredTokens.push_back(t);
	//        }
	//    }
	//}

	//return filteredTokens;
}

// Is token a bracketed input?
bool IsVar(const string& token) {
	return token.size() >= 2 && token.front() == '[' && token.back() == ']';
}

// Is token in specified keyword set?
bool IsInSet(const string& token, const set<string>& keywords) {
	return keywords.find(token) != keywords.end();
}

// Strip brackets for final output
string CleanVar(string token) {
	if (IsVar(token)) {
		return token.substr(1, token.size() - 2);
	}
	return token;
}

// Load cures our dementia
CustomSheet load() {
	return *(new CustomSheet("nada", "nada"));
}

// Save lets us store stuff in files
// Felt like being unique and separated everything with pipes
// *.psv = Pipe Separated Values
void save(string name, string content) {};

// ==================================================================================================
// =                                          CREATION                                              =
// ==================================================================================================

// Check if the character sheet already exists before it's created again
bool CheckForAlreadyExists(string sName, map<string, CustomSheet>& cSheets) {
	// First check in-memory collection
	if (cSheets.find(sName) != cSheets.end()) {
		SetColor(12); cout << "\nERROR: SHEET ";
		SetColor(9); cout << "[";
		SetColor(15); cout << sName;
		SetColor(9); cout << "] ";
		SetColor(12); cout << "ALREADY EXISTS!\n\n";
		return true;
	}

	// Then check the persistent index (pipe-separated values)
	ifstream indexFile("index.psv");
	if (indexFile.is_open()) {
		string line;
		auto trim = [](string &s) {
			size_t start = s.find_first_not_of(" \t\r\n");
			if (start == string::npos) { s.clear(); return; }
			size_t end = s.find_last_not_of(" \t\r\n");
			s = s.substr(start, end - start + 1);
		};

		while (getline(indexFile, line)) {
			if (line.empty()) continue;

			// take the first pipe-separated field as the sheet name
			string sheet = line;
			size_t pipePos = sheet.find('|');
			if (pipePos != string::npos) sheet = sheet.substr(0, pipePos);
			trim(sheet);

			if (!sheet.empty() && ToUpper(sheet) == ToUpper(sName)) {
				SetColor(12); cout << "\nERROR: SHEET ";
				SetColor(9); cout << "[";
				SetColor(15); cout << sName;
				SetColor(9); cout << "] ";
				SetColor(12); cout << "ALREADY EXISTS!\n\n";
				indexFile.close();
				return true;
			}
		}

		indexFile.close();
	}

	// Not found
	return false;
};

// Printing Function to make code shorter
void CreatedSheet(string sName, string cName) {
	SetColor(14); cout << "\nCREATED ";
	SetColor(13); cout << "SHEET ";
	SetColor(9); cout << "[";
	SetColor(15); cout << sName;
	SetColor(9); cout << "] ";
	SetColor(12); cout << "WITH ";
	SetColor(11); cout << "CHARACTER ";
	SetColor(9); cout << "[";
	SetColor(15); cout << cName;
	SetColor(9); cout << "]\n\n";
};

// Parses through creation instruction to construct a new character sheet
// Handles filler words like "A", "NEW", and others
// Several optional fields like 'with character' and 'with fields'
string HandleCreate(string inputString, map<string, CustomSheet>& customSheets) {

	vector<string> inst; // Create pointer for tokenized instructions without reserving the space
	SetFlags(new bool[4]{ 0, 0, 0 }); // Reset program flags

	// MENU DISPLAY
	switch (PROGRAM_FLAGS.NERD) {
	case 0: { // normal person mode
		
		// GUI
		string displayMessage = "";
		string nextModeMessage = "";

		// Input storage
		char c = 0;

		while (!PROGRAM_FLAGS.STEP) {
			cout << CLEAR;

			// PUT DECORATIONS HERE LATER
			
			// debug info
			cout << "selection: " << PROGRAM_FLAGS.SELECTION << "\n";
			cout << "mode: " << PROGRAM_FLAGS.MODE << "\n";
			cout << "step: " << PROGRAM_FLAGS.STEP + 1 << "\n";

			displayMessage = "\nWhat do you want to create?";
			displayMessage += "\n---------------------------";

			// Create Menu Options
			switch (PROGRAM_FLAGS.SELECTION) {
			case 0b000: {
				displayMessage += "\nCharacter Sheet File      >";
				displayMessage += "\nCharacter From Template    ";
				displayMessage += "\nTemplate File              ";
				displayMessage += "\nEnemy From Template        ";
				break;
			}
			case 0b001: {
				displayMessage += "\nCharacter Sheet File       ";
				displayMessage += "\nCharacter From Template   >";
				displayMessage += "\nTemplate File              ";
				displayMessage += "\nEnemy From Template        ";
				break;
			}
			case 0b010: {
				displayMessage += "\nCharacter Sheet File       ";
				displayMessage += "\nCharacter From Template    ";
				displayMessage += "\nTemplate File             >";
				displayMessage += "\nEnemy From Template        ";
				break;
			}
			case 0b011: {
				displayMessage += "\nCharacter Sheet File       ";
				displayMessage += "\nCharacter From Template    ";
				displayMessage += "\nTemplate File              ";
				displayMessage += "\nEnemy From Template       >";
				break;
			}
			default:
				PROGRAM_FLAGS.SELECTION = 0;
				break;
			}

			cout << displayMessage;

			// Navigate start menu
			GetNormalModeMenuInput(c);
			PROGRAM_FLAGS.SELECTION = (c == DOWN) ? ++PROGRAM_FLAGS.SELECTION : ((c == UP) ? --PROGRAM_FLAGS.SELECTION : PROGRAM_FLAGS.SELECTION);
			PROGRAM_FLAGS.MODE = (c == RIGHT) ? PROGRAM_FLAGS.SELECTION : 0;
		}

		// After pressing enter/return, move into the sub-menu for the selected option.
		// This sets the current sub-menu to the same as the menu selection from earlier.
		PROGRAM_FLAGS.STEP++;
		displayMessage += "\n\n---------------------------\n";

        while (PROGRAM_FLAGS.STEP == 2) {

            // STEP 2: Get further input based on selected option
            switch (PROGRAM_FLAGS.MODE) {
            case 0b00: { // CHAR FILE: needs file name, character name, character desc
                string filename, characterName, description;

                cout << "\nEnter File Name: "; // If name of character is given, search for sheet and make filename that + .cha
                cin >> filename;
                cin.ignore(max(filename.length(), INT_MAX), '\n');

                cout << "Enter Character Name: ";
                getline(cin, characterName);

                cout << "Enter Character Description: ";
                getline(cin, description);

                if (CheckForAlreadyExists(filename, customSheets)) {
                    cout << "\nPlease choose a different name.\n";
                    return NULL;
                }

                // Create in-memory sheet and persist to file
                CustomSheet sheet(filename, characterName);
                customSheets.insert({ filename, sheet });

                // write .cha file
                ofstream out(filename + ".cha");
                if (out.is_open()) {
                    out << "SHEET|" << filename << "|" << characterName << "|" << description << "\n";
                    out.close();
                }

                // update index
                ofstream index("index.psv", ios::app);
                if (index.is_open()) {
                    index << filename << "|" << characterName << "|" << description << "\n";
                    index.close();
                }

                CreatedSheet(filename, characterName);
                return "s";
            }
            case 0b01: { // CHAR TMP: needs file name, character name, character desc
                string templateName, characterName, description;

                cout << "\nEnter Template File Name: "; // If name of template sheet is given, search for sheet and make templateName that
                cin >> templateName;
                cin.ignore(max(templateName.length(), INT_MAX), '\n');

                cout << "Enter Character Name: ";
                getline(cin, characterName);

                cout << "Enter Character Description: ";
                getline(cin, description);

                // create sheet in memory
                string outFilename = characterName;
                if (outFilename.empty()) outFilename = templateName;
                if (CheckForAlreadyExists(outFilename, customSheets)) {
                    cout << "\nPlease choose a different name.\n";
                    return NULL;
                }

                CustomSheet sheet(outFilename, characterName);
                customSheets.insert({ outFilename, sheet });

                // write character file named by characterName
                ofstream out(outFilename + ".cha");
                if (out.is_open()) {
                    out << "SHEET|" << outFilename << "|" << characterName << "|" << description << "\n";
                    out.close();
                }

                // append to index
                ofstream index("index.psv", ios::app);
                if (index.is_open()) {
                    index << outFilename << "|" << characterName << "|" << description << "\n";
                    index.close();
                }

                CreatedSheet(outFilename, characterName);
                return "s";
            }
            case 0b10: { // TEMPLATE: needs file name and fields
                string templateName;
                cout << "\nEnter Template Name: ";
                cin >> templateName;
                cin.ignore(max(templateName.length(), INT_MAX), '\n');

                if (CheckForAlreadyExists(templateName, customSheets)) {
                    cout << "\nPlease choose a different name.\n";
                    return NULL;
                }

                CustomSheet tpl(templateName, "nada");

                cout << "Enter template fields (one per line). Leave blank to finish:\n";
                vector<string> templateFields;
                while (true) {
                    string field;
                    cout << "> ";
                    getline(cin, field);
                    if (field.empty()) break;
                    tpl.addCustomField(field, "");
                    templateFields.push_back(field);
                }

                // persist template file
                ofstream out(templateName + ".tpf");
                if (out.is_open()) {
                    out << "TEMPLATE|" << templateName << "\n";
                    for (const auto& f : templateFields) {
                        out << f << "|" << "" << "\n";
                    }
                    out.close();
                }

                // store in-memory
                customSheets.insert({ templateName, tpl });

                // update index
                ofstream index("index.psv", ios::app);
                if (index.is_open()) { index << templateName << "|TEMPLATE|" << "\n"; index.close(); }

                CreatedSheet(templateName, "template");
                return "s";
            }
            case 0b11: { // ENEMY TEMPLATE: needs file name (template) -> creates enemy instance
                string templateName;
                cout << "\nEnter Template Name to instantiate enemy from: ";
                cin >> templateName;
                cin.ignore(max(templateName.length(), INT_MAX), '\n');

                // try to open template file
                ifstream in(templateName + ".tpf");
                if (!in.is_open()) {
                    cout << "\nTemplate not found: " << templateName << "\n";
                    return NULL;
                }

                // generate a simple serial using time
                auto now = time(nullptr);
                string serial = to_string(now);
                string enemyName = templateName + "_ENEMY_" + serial;

                CustomSheet enemy(enemyName, enemyName);

                string line;
                // skip header if present
                while (getline(in, line)) {
                    if (line.rfind("TEMPLATE|", 0) == 0) continue;
                    if (line.empty()) continue;
                    // parse field|value
                    size_t pos = line.find('|');
                    string field = (pos == string::npos) ? line : line.substr(0, pos);
                    enemy.addCustomField(field, "nada");
                }
                in.close();

                // write enemy .cha
                ofstream out(enemyName + ".cha");
                if (out.is_open()) {
                    out << "SHEET|" << enemyName << "|" << enemyName << "|" << "enemy instance" << "\n";
                    out.close();
                }

                customSheets.insert({ enemyName, enemy });

                // update index
                ofstream index("index.psv", ios::app);
                if (index.is_open()) { index << enemyName << "|ENEMY|" << "\n"; index.close(); }

                CreatedSheet(enemyName, enemyName);
                return "s";
            }
            default:
                break;
            }

            return NULL;
        }


		return "s";

		break;
	}
	case 1: { // nerd mode
			if (inputString.empty())
				return GetErrors(2) + "\nEXPECTED INPUT";

			// Convert Global Input to Uppercase
			inputString = ToUpper(inputString);
			// tokenize instruction data
			inst = Tokenize(inputString);

			// If you're in nerd mode, the create instruction takes options like -cs, -cf, -et, and -ei corresponding to the options above
			// "CREATE MODE"
			// > 

			return "s";
			break;
		}
	}

}

// ==================================================================================================
// =                                          DELETION                                              =
// ==================================================================================================

string HandleDelete(string inputString, map<string, CustomSheet>& customSheets) {

	return "s";
}

// ==================================================================================================
// =                                         FRONT DESK                                             =
// ==================================================================================================

// Parses through input text
// Routes instructions to each department
string HandleInput(string inputString, map<string, CustomSheet>& customSheets) {
	// skeleton code while I implement creation functionality
	inputString = ToUpper(inputString);

	if (inputString == "CREATE") {
		return HandleCreate(inputString, customSheets) + "\n";
	}
	else if (inputString == "NERD") {
		PROGRAM_FLAGS.NERD = 1;
		return "Starting NERD MODE...\n\n";
	}
	
	//// cat stepped on the keyboard
	//if (inputString.empty()) return GetErrors(2);

	//// Convert all text to uppercase
	//transform(inputString.begin(), inputString.end(), inputString.begin(), toupper);

	//// Parse string into tokens
	//stringstream ss(inputString);
	//string segment;
	//vector<string> tokens;
	//while (ss >> segment)
	//    tokens.push_back(segment);

	//// Record first and second tokens for use later
	//string first = tokens[0];
	//string second = (tokens.size() > 1) ? tokens[1] : "";

	//// get lost you disgusting vectors...
	//if (!tokens.size() > 0) tokens.erase(tokens.begin());
	//
	//// Use the stupid gross disgusting useless tokens to remove the first keyword from input
	//string restOfInput;
	//for (string s : tokens)
	//    restOfInput += s + " ";

	//// Check Second Keyword Constraint
	//set<string> restrictedSecond = {
	//    "CREATE", "ADD", "INSERT", "BUILD", "FORM", "NEW"
	//    "REMOVE", "DELETE",
	//    "UPDATE", "CHANGE", "MODIFY", "SET",
	//    "READ", "RETURN", "GET"
	//};

	//// If second keyword IS NOT in the list (or doesn't exist), proceed
	//if (restrictedSecond.find(second) == restrictedSecond.end()) {

	//    // Dispatch based on First Keyword
	//    if (IsInSet(first, { "CREATE", "ADD", "INSERT", "BUILD", "FORM", "NEW" })) {
	//        return HandleCreate(restOfInput, customSheets, 0) + "\n";
	//    }
	//    else if (IsInSet(first, { "REMOVE", "DELETE" })) {
	//        return HandleDelete(restOfInput, customSheets);
	//    }
	//    else if (IsInSet(first, { "UPDATE", "CHANGE", "MODIFY", "SET" })) {
	//        //return HandleUpdate(restOfInput, customSheets);
	//    }
	//    else if (IsInSet(first, { "READ", "RETURN", "GET" })) {
	//        //return HandleGet(restOfInput, customSheets);
	//    }
	//}

	//// stoopid idiots didn't read the documentation
	//return GetErrors(2);
}

// Retrieve an error given a specified code
string GetErrors(short i) {
	// change this stupid fucking vector to a fixed array later for optimization
	vector<string> e = {
		"ERROR CODE -1: FIELD DOES NOT EXIST\n",
		"ERROR CODE -2: EXPECTED field OR value FOR UPDATE TYPE\n",
		"ERROR -3: BAD SYNTAX\n" 
		"ERROR -4: SECRET FOUND\n" };
	return e[i];
}