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

// --- Helper: Uppercase Conversion ---
string ToUpper(const string& str) {
    string s = str;
    transform(s.begin(), s.end(), s.begin(), toupper);
    return s;
}

// --- Helper: Tokenizer ---
// 1. Handles square brackets [Variable Input] as single tokens.
// 2. Removes "A".
// 3. Removes flowchart X'd out words (NEW, BUT, EXCEPT, AS, EXACTLY, ETC).
vector<string> TokenizeAndClean(const string& input) {
    vector<string> tokens;
    string currentToken;
    bool insideBrackets = false;

    // Set of words to ignore completely based on flowchart X's
    set<string> ignoredWords = {
        "NEW", "BUT", "EXCEPT", "AS", "EXACTLY", "ETC", "A"
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
            // or just treat as part of word. Flowchart says "END / ."
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
        // If it's a bracketed variable, keep it (and keep case if desired, 
        // though prompt asked to uppercase everything at start).
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

// --- Helper: Check if token is a bracketed variable ---
bool IsVar(const string& token) {
    return token.size() >= 2 && token.front() == '[' && token.back() == ']';
}

// --- Helper: Strip brackets for final output ---
string CleanVar(string token) {
    if (IsVar(token)) {
        return token.substr(1, token.size() - 2);
    }
    return token;
}

// --- Helper: Check Set Membership ---
bool IsInSet(const string& token, const set<string>& keywords) {
    return keywords.find(token) != keywords.end();
}

// ==========================================
// MAIN LOGIC: HandleCreate
// ==========================================
string HandleCreate(string inputString, map<string, CustomSheet>& customSheets) {
    bool inputtingCharacter = true;
    bool inputtingFields = true;
    
    // 1. Convert Global Input to Uppercase
    inputString = ToUpper(inputString);

    // 2. Tokenize (This handles ignoring NEW, A, etc., and grouping [brackets])
    vector<string> tokens = TokenizeAndClean(inputString);
    int pos = 0;
    int max = tokens.size();

    // --- Flowchart Start ---
    // Check: CREATE/ADD/INSERT/BUILD/FORM
    if (pos >= max || !IsInSet(tokens[pos++], { "CREATE", "ADD", "INSERT", "BUILD", "FORM", "NEW"}))
        return GetErrors(2);

    // Check: SHEET/FORM
    if (pos >= max || !IsInSet(tokens[pos++], { "SHEET", "FORM" }))
        return GetErrors(2);

    // --- BRANCH 1: SHEET NAME ---
    string sheetName = "nada";

    if (pos < max) {
        string t = tokens[pos];

        // Path A: CALLED/NAMED/TITLED -> [Var]
        if (IsInSet(t, { "CALLED", "NAMED", "TITLED" })) {
            pos++;
            if (pos < max && IsVar(tokens[pos])) {
                sheetName = CleanVar(tokens[pos++]);
            }
            else {
                return GetErrors(2); // Expected brackets
            }
        }
        // Path B: WITHOUT/MINUS/EXCLUDING -> TITLE/NAME/HEAD... -> (Sets to "nada")
        else if (IsInSet(t, { "WITHOUT", "MINUS", "EXCLUDING" })) {
            pos++;
            if (pos < max && IsInSet(tokens[pos], { "TITLE", "NAME", "HEAD", "SHEETTITLE", "SHEETNAME", "SHEETHEAD", "SHEET_TITLE", "SHEET_NAME", "SHEET_HEAD" })) {
                pos++;
                sheetName = "nada";
            }
            else if (pos < max && IsInSet(tokens[pos], { "CHARACTER", "CREATURE", "ENTITY" })) {
                inputtingCharacter = false;
			}
            // Chart also implies WITHOUT -> CHAR (skipping sheet title logic). 
            // If we see CHAR next, we do nothing here, sheetName remains "nada" default.
        }
        // Path C: WITH/PLUS/INCLUDING -> TITLE... -> [Var]
        else if (IsInSet(t, { "WITH", "PLUS", "INCLUDING", "CONTAINING" })) {
            // Look ahead: is the next word TITLE? If so, we are naming the sheet.
            // If the next word is CHARACTER, we are skipping the sheet name (sheetName remains default "nada").
            if (pos + 1 < max && IsInSet(tokens[pos + 1], { "TITLE", "NAME", "HEAD", "SHEETTITLE", "SHEETNAME", "SHEETHEAD", "SHEET_TITLE", "SHEET_NAME", "SHEET_HEAD" })) {
                pos += 2; // Skip WITH and TITLE
                if (pos < max && IsVar(tokens[pos])) {
                    sheetName = CleanVar(tokens[pos++]);
                }
                else {
                    return GetErrors(2);
                }
            }
        }
    }
    
    string charName = "nada";
    if (pos >= max) {
        if (customSheets.find(sheetName) != customSheets.end()) {
            setColor(12); cout << "\nERROR: SHEET ";
            setColor(9); cout << "[";
            setColor(15); cout << sheetName;
            setColor(9); cout << "] ";
            setColor(12); cout << "ALREADY EXISTS!\n\n";
            return "";
		}
        
        CustomSheet newSheet(sheetName, charName);
        customSheets.insert({ newSheet.getSheetName(), newSheet });
        
        setColor(14); cout << "\nCREATED ";
        setColor(13); cout << "SHEET ";
        setColor(9); cout << "[";
        setColor(15); cout << sheetName;
        setColor(9); cout << "] ";
        setColor(12); cout << "WITH ";
        setColor(13); cout << "CHARACTER ";
        setColor(9); cout << "[";
        setColor(15); cout << charName << "\n\n";

        return "";
    }

    if (inputtingCharacter) {
        // --- CONNECTOR TO CHARACTER ---
        // We might have a connector word here: WITH/PLUS/INCLUDING/AND/HAVING
        // Note: In Path C, we might have already consumed "WITH". 
        // We check if the *current* token is a connector.
        if (pos < max && IsInSet(tokens[pos], { "WITH", "PLUS", "INCLUDING", "AND", "HAVING", "CONTAINING" })) {
            pos++;
        }

        // Check: CHARACTER/CREATURE/ENTITY
        if (pos >= max || !IsInSet(tokens[pos++], { "CHARACTER", "CREATURE", "ENTITY" }))
            return GetErrors(2);

        // --- BRANCH 2: CHARACTER NAME ---
        if (pos < max) {
            string t = tokens[pos];

            // Path A: CALLED/NAMED/TITLED -> [Var]
            if (IsInSet(t, { "CALLED", "NAMED", "TITLED" })) {
                pos++;
                if (pos < max && IsVar(tokens[pos])) {
                    charName = CleanVar(tokens[pos++]);
                }
                else {
                    return GetErrors(2);
                }
            }
            // Path B: WITHOUT/MINUS/EXCLUDING -> TITLE... -> (Sets to "nada")
            else if (IsInSet(t, { "WITHOUT", "MINUS", "EXCLUDING" })) {
                pos++;
                if (pos < max && IsInSet(tokens[pos], { "TITLE", "NAME", "HEAD", "SHEETTITLE", "SHEETNAME", "SHEETHEAD", "SHEET_TITLE", "SHEET_NAME", "SHEET_HEAD" })) {
                    pos++;
                    charName = "nada";
                }
                // If next was something else, we assume defaults.
            }
            // Path C: WITH/PLUS/INCLUDING -> TITLE... -> [Var]
            else if (IsInSet(t, { "WITH", "PLUS", "INCLUDING", "CONTAINING" })) {
                // Look ahead
                if (pos + 1 < max && IsInSet(tokens[pos + 1], { "TITLE", "NAME", "HEAD", "SHEETTITLE", "SHEETNAME", "SHEETHEAD", "SHEET_TITLE", "SHEET_NAME", "SHEET_HEAD" })) {
                    pos += 2;
                    if (pos < max && IsVar(tokens[pos])) {
                        charName = CleanVar(tokens[pos++]);
                    }
                    else {
                        return GetErrors(2);
                    }
                }
            }
        }
    }

    // --- CONNECTOR TO FIELDS ---
    if (pos < max && IsInSet(tokens[pos], { "WITH", "PLUS", "INCLUDING", "AND", "HAVING", "CONTAINING" })) {
        pos++;
    }

    if (pos >= max) {
        if (customSheets.find(sheetName) != customSheets.end()) {
            setColor(12); cout << "\nERROR: SHEET ";
            setColor(9); cout << "[";
            setColor(15); cout << sheetName;
            setColor(9); cout << "] ";
            setColor(12); cout << "ALREADY EXISTS!\n\n";
            return "";
        }
        
        CustomSheet newSheet(sheetName, charName);
        customSheets.insert({ newSheet.getSheetName(), newSheet });

        inputtingFields = false;

        setColor(14); cout << "\nCREATED ";
        setColor(13); cout << "SHEET ";
        setColor(9); cout << "[";
        setColor(15); cout << sheetName;
        setColor(9); cout << "] ";
        setColor(12); cout << "WITH ";
        setColor(11); cout << "CHARACTER ";
        setColor(9); cout << "[";
        setColor(15); cout << charName;
        setColor(9); cout << "]\n\n";
    }

    string result = "CREATED SHEET: " + sheetName + "\nCHARACTER: " + charName;
    if (inputtingFields)
    {
        // Check: FIELDS
        if (tokens[pos++] != "FIELDS")
            return GetErrors(2);

        // --- FIELDS LOOP ---
        // Expected format: [FieldName] [FieldValue] (OPTIONAL: AND/PLUS/INCLUDING/,)
        result = "CREATED SHEET: " + sheetName + "\nCHARACTER: " + charName + "\nFIELDS:\n";

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
                }
                else {
                    return GetErrors(2); // Missing Value
                }

                // 3. Optional Separator (AND/PLUS/INCLUDING/,)
                if (pos < max && IsInSet(tokens[pos], { "AND", "PLUS", "INCLUDING", "," })) {
                    pos++;
                }
            }
            else {
                // If we hit something that isn't a bracket and isn't END/., it's junk
                return GetErrors(2);
            }
        }

        if (customSheets.find(sheetName) != customSheets.end()) {
            setColor(12); cout << "\nERROR: SHEET ";
            setColor(9); cout << "[";
            setColor(15); cout << sheetName;
            setColor(9); cout << "] ";
            setColor(12); cout << "ALREADY EXISTS!\n\n";
            return "";
        }
    }

    vector<string> words;
	words = TokenizeAndClean(result);

    setColor(14); cout << "\nCREATED";
    setColor(13); cout << " SHEET";
    setColor(9); cout << " [";
    setColor(15); cout << sheetName;
    setColor(9); cout << "]\n";
    setColor(12); cout << "WITH ";
    setColor(11); cout << "CHARACTER";
    setColor(9); cout << " [";
    setColor(15); cout << charName;
    setColor(9); cout << "]\n";

    return "SUCCESS!\n\n";
}

string GetErrors(short i) {
    // change this stupid fucking vector to a fixed array later for optimization
    vector<string> e = {
        "ERROR CODE -1: FIELD DOES NOT EXIST",
        "ERROR CODE -2: EXPECTED field OR value FOR UPDATE TYPE",
        "ERROR -3: BAD SYNTAX" };
    return e[i];
}

void setColor(short color) { SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); }

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
        "CREATE", "ADD", "INSERT",
        "REMOVE", "DELETE",
        "UPDATE", "CHANGE", "SET",
        "GET", "READ", "RETURN"
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

// Load cures our dementia
CustomSheet load() {
    return *(new CustomSheet("nada", "nada"));
}

// Save lets us store stuff in files
// Felt like being unique and separated everything with pipes
// *.psv = Pipe Separated Values
void save() {};


