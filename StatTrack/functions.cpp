#pragma once

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


// ==================================================================================================
// =                                           HELPERS                                              =
// ==================================================================================================

// Set the current text and cell color
void SetColor(short color) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); }

// Convert parameter into uppercase
string ToUpper(const string& str) {
    string s = str;
    transform(s.begin(), s.end(), s.begin(), toupper);
    return s;
}

// --- Tokenizer ---
// Handles square brackets [Variable Input] as single tokens.
// Removes select keywords (A, NEW, BUT, EXCEPT, AS, EXACTLY).
vector<string> TokenizeAndClean(const string& input) {
    vector<string> tokens;
    string currentToken;
    bool insideBrackets = false;

    // Set of words to ignore completely based on flowchart X's
    set<string> ignoredWords = {
        "NEW", "BUT", "EXCEPT", "AS", "EXACTLY", "A"
    };

    for (size_t i = 0; i < input.length(); ++i) {
        char c = input[i];

        if (c == '[') {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
            insideBrackets = true;
            currentToken += c;
        }
        else if (c == ']') {
            currentToken += c;
            insideBrackets = false;
            tokens.push_back(currentToken);
            currentToken.clear();
        }
        else if (isspace(c) && !insideBrackets) {
            if (!currentToken.empty()) {
                tokens.push_back(currentToken);
                currentToken.clear();
            }
        }
        else {
            // Handle period at end of sentence as distinct token if needed, 
            // or just treat as part of word.
            if (c == '.' && !insideBrackets) {
                if (!currentToken.empty()) {
                    tokens.push_back(currentToken);
                    currentToken.clear();
                }
                tokens.push_back(".");
            }
            else {
                currentToken += c;
            }
        }
    }
    if (!currentToken.empty()) tokens.push_back(currentToken);

    // Filter phase
    vector<string> filteredTokens;
    for (const auto& t : tokens) {
        // Keep bracketed variables
        if (t.front() == '[' && t.back() == ']') {
            filteredTokens.push_back(t);
        }
        else {
            // It's a keyword
            if (ignoredWords.find(t) == ignoredWords.end()) {
                filteredTokens.push_back(t);
            }
        }
    }

    return filteredTokens;
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
void save() {};

// ==================================================================================================
// =                                          CREATION                                              =
// ==================================================================================================

// Check if the character sheet already exists before it's created again
bool CheckForAlreadyExists(string sName, map<string, CustomSheet>& cSheets) {
    if (cSheets.find(sName) != cSheets.end()) {
        SetColor(12); cout << "\nERROR: SHEET ";
        SetColor(9); cout << "[";
        SetColor(15); cout << sName;
        SetColor(9); cout << "] ";
        SetColor(12); cout << "ALREADY EXISTS!\n\n";
        return true;
    }
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

    bool inputtingCharacter = true; // Are we inputting a character name?
    bool inputtingFields = true; // Are we inputting data fields?
    
    // Convert Global Input to Uppercase
    inputString = ToUpper(inputString);

    // Handle ignored words and grouped brackets
    vector<string> tokens = TokenizeAndClean(inputString);
    int pos = 0;
    int max = tokens.size();
    
    // if CREATE/ADD/INSERT/BUILD/FORM
    if (pos >= max || !IsInSet(tokens[pos++], { "CREATE", "ADD", "INSERT", "BUILD", "FORM", "NEW"}))
        return GetErrors(2);

    // if SHEET/FORM
    if (pos >= max || !IsInSet(tokens[pos++], { "SHEET", "FORM" }))
        return GetErrors(2);

    // default: "nada"
    string sheetName = "nada";

    if (pos < max) {
        string t = tokens[pos]; // current token

        // if CALLED/NAMED/TITLED -> [var]
        if (IsInSet(t, { "CALLED", "NAMED", "TITLED" })) {
            pos++;
            if (pos < max && IsVar(tokens[pos]))
                sheetName = CleanVar(tokens[pos++]);
            else
                return GetErrors(2); // Expected brackets
        }
        // if WITHOUT/MINUS/EXCLUDING -> TITLE/NAME/HEAD...
        else if (IsInSet(t, { "WITHOUT", "MINUS", "EXCLUDING" })) {
            pos++;
            if (pos < max && IsInSet(tokens[pos], { "TITLE", "NAME", "HEAD", "SHEETTITLE", "SHEETNAME", "SHEETHEAD", "SHEET_TITLE", "SHEET_NAME", "SHEET_HEAD" }))
                pos++;
            else if (pos < max && IsInSet(tokens[pos], { "CHARACTER", "CREATURE", "ENTITY" }))
                inputtingCharacter = false;
        }
        // WITH/PLUS/INCLUDING -> TITLE... -> [Var]
        else if (IsInSet(t, { "WITH", "PLUS", "INCLUDING", "CONTAINING" })) {
            // If next word -> [title or a synonym]
            if (pos + 1 < max && IsInSet(tokens[pos + 1], { "TITLE", "NAME", "HEAD", "SHEETTITLE", "SHEETNAME", "SHEETHEAD", "SHEET_TITLE", "SHEET_NAME", "SHEET_HEAD" })) {
                pos += 2; // Skip WITH and TITLE

                if (pos < max && IsVar(tokens[pos]))
                    sheetName = CleanVar(tokens[pos++]);
                else
                    return GetErrors(2);
            }
        }
    }
    
    // default: "nada"
    string charName = "nada";

    if (pos >= max) { // If we have nothing past the sheet name
        if (CheckForAlreadyExists(sheetName, customSheets)) return GetErrors(2);
        
        CustomSheet newSheet(sheetName, charName);
        customSheets.insert({ newSheet.getSheetName(), newSheet });
        
        CreatedSheet(sheetName, charName);

        inputtingCharacter = false;
    }

    if (inputtingCharacter) {
        // if *current* token is a connector between sheet and character.
        if (pos < max && IsInSet(tokens[pos], { "WITH", "PLUS", "INCLUDING", "AND", "HAVING", "CONTAINING" }))
            pos++;

        // if CHARACTER/CREATURE/ENTITY
        if (pos >= max || !IsInSet(tokens[pos++], { "CHARACTER", "CREATURE", "ENTITY" }))
            return GetErrors(2);

        // --- BRANCH 2: CHARACTER NAME ---
        if (pos < max) {
            string t = tokens[pos];

            // if CALLED/NAMED/TITLED -> [Var]
            if (IsInSet(t, { "CALLED", "NAMED", "TITLED" })) {
                pos++;
                if (pos < max && IsVar(tokens[pos]))
                    charName = CleanVar(tokens[pos++]);
                else
                    return GetErrors(2);
            }
            // if WITHOUT/MINUS/EXCLUDING -> TITLE... -> (Sets to "nada")
            else if (IsInSet(t, { "WITHOUT", "MINUS", "EXCLUDING" })) {
                pos++;
                if (pos < max && IsInSet(tokens[pos], { "TITLE", "NAME", "HEAD", "SHEETTITLE", "SHEETNAME", "SHEETHEAD", "SHEET_TITLE", "SHEET_NAME", "SHEET_HEAD" })) {
                    pos++;
                    charName = "nada";
                }
            }
            // if WITH/PLUS/INCLUDING -> TITLE... -> [Var]
            else if (IsInSet(t, { "WITH", "PLUS", "INCLUDING", "CONTAINING" })) {
                // Look ahead
                if (pos + 1 < max && IsInSet(tokens[pos + 1], { "TITLE", "NAME", "HEAD", "SHEETTITLE", "SHEETNAME", "SHEETHEAD", "SHEET_TITLE", "SHEET_NAME", "SHEET_HEAD" })) {
                    pos += 2;
                    if (pos < max && IsVar(tokens[pos]))
                        charName = CleanVar(tokens[pos++]);
                    else
                        return GetErrors(2);
                }
            }
        }

        // --- CONNECTOR TO FIELDS ---
        if (pos < max && IsInSet(tokens[pos], { "WITH", "PLUS", "INCLUDING", "AND", "HAVING", "CONTAINING" }))
            pos++;

        if (pos >= max) {
            if (CheckForAlreadyExists(sheetName, customSheets)) return "";

            CustomSheet newSheet(sheetName, charName);
            customSheets.insert({ newSheet.getSheetName(), newSheet });

            inputtingFields = false;
        }

        string result = "CREATED SHEET: " + sheetName + "\nCHARACTER: " + charName;
        if (inputtingFields) {
            map<string, string> fields;

            // Check: FIELDS
            if (tokens[pos++] != "FIELDS")
                return GetErrors(2);

            // --- FIELDS LOOP ---
            // Expected format: [FieldName] [FieldValue] (OPTIONAL: AND/PLUS/INCLUDING/,)
            result += "\nFIELDS:\n";

            while (pos < max) {
                // Check for termination words
                if (IsInSet(tokens[pos], { "END", "." })) break;

                // 1. Field Name
                if (IsVar(tokens[pos])) {
                    string fName = CleanVar(tokens[pos++]);

                    // 2. Field Value
                    if (pos < max && IsVar(tokens[pos])) {
                        string fVal = CleanVar(tokens[pos++]);
                        result += " - " + fName + ": " + fVal + "\n";

                        fields.insert({ fName, fVal });
                    }
                    else
                        return GetErrors(2); // Missing Value

                    // 3. Optional Separator (AND/PLUS/INCLUDING/,)
                    if (pos < max && IsInSet(tokens[pos], { "AND", "PLUS", "INCLUDING", "," }))
                        pos++;
                }
                else
                    return GetErrors(2); // If we hit something that isn't a bracket and isn't END/., it's junk
            }

            if (CheckForAlreadyExists(sheetName, customSheets)) return "";

            CustomSheet newSheet(sheetName, charName);
            for (pair<string, string> fieldData : fields)
                newSheet.addCustomField(fieldData.first, fieldData.second);

            customSheets.insert({ sheetName, newSheet });

            CreatedSheet(sheetName, charName);
        }
    }

    return "SUCCESS!\n\n";
}


// ==================================================================================================
// =                                          FRONT DESK                                            =
// ==================================================================================================

// Parses through input text
// Routes instructions to each department
string HandleInput(string inputString, map<string, CustomSheet>& customSheets) {
    // cat stepped on the keyboard
    if (inputString.empty()) return GetErrors(2);

    // Convert all text to uppercase
    transform(inputString.begin(), inputString.end(), inputString.begin(), toupper);

    // Parse string into tokens
    stringstream ss(inputString);
    string segment;
    vector<string> tokens;
    while (ss >> segment)
        tokens.push_back(segment);

    // Record first and second tokens for use later
    string first = tokens[0];
    string second = (tokens.size() > 1) ? tokens[1] : "";

    // get lost you disgusting vectors...
    if (!tokens.size() > 0) tokens.erase(tokens.begin());
    
    // Use the stupid gross disgusting useless tokens to remove the first keyword from input
    string restOfInput;
    for (string s : tokens)
        restOfInput += s + " ";

    // Check Second Keyword Constraint
    set<string> restrictedSecond = {
        "CREATE", "ADD", "INSERT", "BUILD", "FORM", "NEW"
        "REMOVE", "DELETE",
        "UPDATE", "CHANGE", "MODIFY", "SET",
        "READ", "RETURN", "GET"
    };

    // If second keyword IS NOT in the list (or doesn't exist), proceed
    if (restrictedSecond.find(second) == restrictedSecond.end()) {

        // Dispatch based on First Keyword
        if (IsInSet(first, { "CREATE", "ADD", "INSERT", "BUILD", "FORM", "NEW" })) {
            return HandleCreate(restOfInput, customSheets) + "\n";
        }
        else if (IsInSet(first, { "REMOVE", "DELETE"})) {
            //return HandleDelete(restOfInput, customSheets);
        }
        else if (IsInSet(first, { "UPDATE", "CHANGE", "MODIFY", "SET"})) {
            //return HandleUpdate(restOfInput, customSheets);
        }
        else if (IsInSet(first, { "READ", "RETURN", "GET" })) {
            //return HandleGet(restOfInput, customSheets);
        }
    }

    // stoopid idiots didn't read the documentation
    return GetErrors(2);
}

// Retrieve an error given a specified code
string GetErrors(short i) {
    // change this stupid fucking vector to a fixed array later for optimization
    vector<string> e = {
        "ERROR CODE -1: FIELD DOES NOT EXIST",
        "ERROR CODE -2: EXPECTED field OR value FOR UPDATE TYPE",
        "ERROR -3: BAD SYNTAX" };
    return e[i];
}