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
        "NEW", "BUT", "EXCEPT", "AS", "EXACTLY", "A", "NAMED", "CALLED", "TITLED"
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
string HandleCreate(string inputString, map<string, CustomSheet>& customSheets, bool mode) {

    bool inputtingCharacter = true; // Are we inputting a character name?
    bool inputtingFields = true; // Are we inputting data fields?

    // create new sheet with character named [name one]

    vector<string> inst; // Create pointer for tokenized instructions without reserving the space
    
    switch (mode) {
    case 0: // normal person mode
        if (inputString.empty())
            return GetErrors(2) + "\nEXPECTED INPUT";

        // Convert Global Input to Uppercase
        inputString = ToUpper(inputString);

        // tokenize instruction data
        inst = TokenizeAndClean(inputString);

        // "What do you want to create?"
        // > Character Sheet File
        // > Character Sheet Field
        // > New Enemy Type File
        // > New Enemy Instance File

        return "s";
        

        break;
    case 1: // nerd mode
        
        // If you're in nerd mode, the create instruction takes options like -cs, -cf, -et, and -ei corresponding to the options above
        // "CREATE MODE"
        // > 

        return "s";
        break;
    }

}

// ==================================================================================================
// =                                          DELETION                                              =
// ==================================================================================================

string HandleDelete(string inputString, map<string, CustomSheet>& customSheets) {

    
}

// ==================================================================================================
// =                                         FRONT DESK                                             =
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
            return HandleCreate(restOfInput, customSheets, 0) + "\n";
        }
        else if (IsInSet(first, { "REMOVE", "DELETE" })) {
            return HandleDelete(restOfInput, customSheets);
        }
        else if (IsInSet(first, { "UPDATE", "CHANGE", "MODIFY", "SET" })) {
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
        "ERROR CODE -1: FIELD DOES NOT EXIST\n",
        "ERROR CODE -2: EXPECTED field OR value FOR UPDATE TYPE\n",
        "ERROR -3: BAD SYNTAX\n" 
        "ERROR -4: SECRET FOUND\n" };
    return e[i];
}