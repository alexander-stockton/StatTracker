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
    if (pos >= max || !IsInSet(tokens[pos++], { "CREATE", "ADD", "INSERT", "BUILD", "FORM", "NEW" }))
        return GetErrors(2);

    // if SHEET/FORM
    if (pos >= max || !IsInSet(tokens[pos++], { "SHEET", "FORM" }))
        return GetErrors(2);

    // default: "nada"
    string sheetName = "nada";

    // --- BRANCH 1: SHEET NAME ---
    if (pos < max) {
        // Optional: Ignore naming keywords if present
        if (IsInSet(tokens[pos], { "CALLED", "NAMED", "TITLED" })) {
            pos++;
        }

        // Check if we have a valid token left after skipping
        if (pos < max) {
            string t = tokens[pos]; // current token

            // 1. Direct Variable (e.g., "[SheetName]")
            if (IsVar(t)) {
                sheetName = CleanVar(tokens[pos++]);
            }
            // 2. WITHOUT/MINUS/EXCLUDING -> TITLE...
            else if (IsInSet(t, { "WITHOUT", "MINUS", "EXCLUDING" })) {
                pos++;
                if (pos < max && IsInSet(tokens[pos], { "TITLE", "NAME", "HEAD", "SHEETTITLE", "SHEETNAME", "SHEETHEAD", "SHEET_TITLE", "SHEET_NAME", "SHEET_HEAD" }))
                    pos++;
                else if (pos < max && IsInSet(tokens[pos], { "CHARACTER", "CREATURE", "ENTITY" }))
                    inputtingCharacter = false;
            }
            // 3. WITH/PLUS/INCLUDING -> TITLE... -> [Var]
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
            // Optional: Ignore naming keywords if present
            if (IsInSet(tokens[pos], { "CALLED", "NAMED", "TITLED" })) {
                pos++;
            }

            if (pos < max) {
                string t = tokens[pos];

                // 1. Direct Variable (e.g., "[CharName]")
                if (IsVar(t)) {
                    charName = CleanVar(tokens[pos++]);
                }
                // 2. WITHOUT/MINUS/EXCLUDING -> TITLE... -> (Sets to "nada")
                else if (IsInSet(t, { "WITHOUT", "MINUS", "EXCLUDING" })) {
                    pos++;
                    if (pos < max && IsInSet(tokens[pos], { "TITLE", "NAME", "HEAD", "SHEETTITLE", "SHEETNAME", "SHEETHEAD", "SHEET_TITLE", "SHEET_NAME", "SHEET_HEAD" })) {
                        pos++;
                        charName = "nada";
                    }
                }
                // 3. WITH/PLUS/INCLUDING -> TITLE... -> [Var]
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
        }

        // --- CONNECTOR TO FIELDS ---
        if (pos < max && IsInSet(tokens[pos], { "WITH", "PLUS", "INCLUDING", "AND", "HAVING", "CONTAINING" }))
            pos++;

        if (pos >= max) {
            if (CheckForAlreadyExists(sheetName, customSheets)) return "";

            CustomSheet newSheet(sheetName, charName);
            customSheets.insert({ newSheet.getSheetName(), newSheet });

            CreatedSheet(sheetName, charName);

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
    return "\nSUCCESS!\n";
}

// ==================================================================================================
// =                                          DELETION                                              =
// ==================================================================================================

string HandleDelete(string inputString, map<string, CustomSheet>& customSheets) {

    // 1. Preparation & Cleaning
    inputString = ToUpper(inputString);
    vector<string> tokens = TokenizeAndClean(inputString);

    if (tokens.empty()) return GetErrors(3);

    // 2. Validate First Keyword
    if (!IsInSet(tokens[0], { "REMOVE", "DELETE" }))
        return GetErrors(3);

    // 3. Validate Second Keyword exists
    if (tokens.size() < 2) return GetErrors(3);

    string second = tokens[1];

    // 4. Proximity Logic Helper
    auto getClosestVar = [&](const set<string>& keywords) -> string {
        int minDistance = 9999;
        string bestMatch = "";
        for (size_t k = 0; k < tokens.size(); ++k) {
            if (IsInSet(tokens[k], keywords)) {
                for (size_t v = 0; v < tokens.size(); ++v) {
                    if (IsVar(tokens[v])) {
                        int dist = abs((int)k - (int)v);
                        if (dist < minDistance) {
                            minDistance = dist;
                            bestMatch = CleanVar(tokens[v]);
                        }
                    }
                }
            }
        }
        return bestMatch;
        };

    // ==========================================
    // BRANCH A: SHEET DELETION (Key Lookup)
    // ==========================================
    if (IsInSet(second, { "SHEET", "FORM" })) {
        string target = getClosestVar({ "SHEET", "FORM" });
        if (target == "") return GetErrors(3);

        auto it = customSheets.find(target);
        if (it != customSheets.end()) {
            customSheets.erase(it);
            SetColor(14); cout << "\nDELETED ";
            SetColor(13); cout << "SHEET ";
            SetColor(9); cout << "[";
            SetColor(15); cout << target;
            SetColor(9); cout << "]\n";
            return "finished!";
        }
        else {
            SetColor(12); cout << "\nSHEET [" << target << "] NOT FOUND.\n";
            return "";
        }
    }

    // ==========================================
    // BRANCH B: CHARACTER DELETION (Search)
    // ==========================================
    else if (IsInSet(second, { "CHARACTER", "CHAR", "PLAYER" })) {
        string target = getClosestVar({ "CHARACTER", "CHAR", "PLAYER" });
        if (target == "") return GetErrors(3);

        // Linear search for character name
        bool found = false;
        for (auto it = customSheets.begin(); it != customSheets.end(); ++it) {
            if (it->second.getCharacterName() == target) {
                customSheets.erase(it);
                found = true;
                break;
            }
        }

        if (found) {
            SetColor(14); cout << "\nDELETED ";
            SetColor(11); cout << "CHARACTER ";
            SetColor(9); cout << "[";
            SetColor(15); cout << target;
            SetColor(9); cout << "]\n";
            return "finished!";
        }
        else {
            SetColor(12); cout << "\nCHARACTER [" << target << "] NOT FOUND.\n";
            return "";
        }
    }

    // ==========================================
    // BRANCH C: FIELD / VALUE / BATCH DELETION
    // ==========================================
    else {
        set<string> sheetKw = { "SHEET", "FORM", "CHARACTER", "CHAR", "PLAYER" };
        set<string> fieldKw = { "FIELD", "FIELDS" };
        set<string> valueKw = { "VALUE", "VALUES", "DATA" };
        set<string> allKw = { "ALL", "EVERY" };

        bool intentValue = false;
        bool intentField = false;
        bool intentAll = false;

        for (const string& t : tokens) {
            if (IsInSet(t, fieldKw)) intentField = true;
            if (IsInSet(t, valueKw)) intentValue = true;
            if (IsInSet(t, allKw))   intentAll = true;
        }

        // Default to Sheet proximity for target
        string targetSheet = getClosestVar(sheetKw);
        string targetField = "";

        // If not deleting ALL, we need the field name
        if ((intentField || intentValue) && !intentAll) {
            targetField = getClosestVar(fieldKw);
        }

        if (targetSheet == "") return GetErrors(3);

        auto it = customSheets.find(targetSheet);
        if (it == customSheets.end()) {
            SetColor(12); cout << "\nSHEET [" << targetSheet << "] NOT FOUND.\n";
            return "";
        }

        // 1. Delete ALL
        if (intentAll) {
            if (intentValue) it->second.resetAllCustomFieldValues();
            else it->second.removeAllCustomFields();
            return "finished!";
        }
        // 2. Delete Single Value
        else if (intentValue) {
            if (targetField == "") return GetErrors(3);
            it->second.updateCustomField("VALUE", targetField, "nada");
            return "finished!";
        }
        // 3. Delete Single Field
        else if (intentField) {
            if (targetField == "") return GetErrors(3);
            it->second.removeCustomField(targetField);
            return "finished!";
        }
    }

    return GetErrors(3);
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
        "ERROR -3: BAD SYNTAX\n" };
    return e[i];
}