#pragma once

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#pragma once

#include "CustomSheet.h"
#include <string>
#include <vector>
#include <set>

using namespace std;

// ==================================================================================================
// =                                           HELPERS                                              =
// ==================================================================================================

// Set the current text and cell color
void SetColor(short);

// Convert parameter into uppercase
string ToUpper(const string&);

// --- Tokenizer ---
// Handles square brackets [Variable Input] as single tokens.
// Removes select keywords (A, NEW, BUT, EXCEPT, AS, EXACTLY).
vector<string> TokenizeAndClean(const string&);

// Is token a bracketed input?
bool IsVar(const string&);

// Is token in specified keyword set?
bool IsInSet(const string&, const set<string>&);

// Strip brackets for final output
string CleanVar(string);

// Load cures our dementia
CustomSheet load();

// Save lets us store stuff in files
// Felt like being unique and separated everything with pipes
// *.psv = Pipe Separated Values
void save();


// ==================================================================================================
// =                                          CREATION                                              =
// ==================================================================================================

// Printing Function to make code shorter
bool CheckForAlreadyExists(string, map<string, CustomSheet>&);

// Printing Function to make code shorter
void CreatedSheet(string, string);

// Parses through creation instruction to construct a new character sheet
// Handles filler words like "A", "NEW", and others
// Several optional fields like 'with character' and 'with fields'
string HandleCreate(string, map<string, CustomSheet>&);


// ==================================================================================================
// =                                          FRONT DESK                                            =
// ==================================================================================================

// Parses through input text
// Routes instructions to each department
string HandleInput(string, map<string, CustomSheet>&);

// Retrieve an error given a specified code
string GetErrors(short i);


#endif // !FUNCTIONS_H