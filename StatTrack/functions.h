#pragma once

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#pragma once

#include "CustomSheet.h"
#include <string>
#include <vector>
#include <set>

using namespace std;

// --- Helper: Uppercase Conversion ---
string ToUpper(const string& str);

// --- Helper: Tokenizer ---
// 1. Handles square brackets [Variable Input] as single tokens.
// 2. Removes "A".
// 3. Removes flowchart X'd out words (NEW, BUT, EXCEPT, AS, EXACTLY, ETC).
vector<string> TokenizeAndClean(const string& input);

// --- Helper: Check if token is a bracketed variable ---
bool IsVar(const string& token);

// --- Helper: Strip brackets for final output ---
string CleanVar(string token);

// --- Helper: Check Set Membership ---
bool IsInSet(const string& token, const set<string>& keywords);

// ==========================================
// MAIN LOGIC: HandleCreate
// ==========================================
string HandleCreate(string inputString, map<string, CustomSheet>& customSheets);

// Listens to your sticky fingers
string HandleInput(string inputString, map<string, CustomSheet>& customSheets);

// you stoopid
string GetErrors(short i);

// this is neat
void setColor(short color);

// Load cures our dementia
CustomSheet load();

// Save lets us store stuff in files
// Felt like being unique and separated everything with pipes
// *.psv = Pipe Separated Values
void save();

#endif // !FUNCTIONS_H