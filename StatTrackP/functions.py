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

from pynput import keyboard

# ==================================================================================================
# =                                    CONSTANT EXPRESSIONS                                        =
# ==================================================================================================
CLEAR = "\x1b[2J\x1b[H"
DOWN = 80
UP = 72
RIGHT = 77

# MENU FLAGS
PROGRAM_FLAGS = {
    "SELECTION": 0b00,
    "MODE": 0b00,
    "STEP": 0b000,
    "SELECTED": 0b0
}

# ==================================================================================================
# =                                          HELPERS                                               =
# ==================================================================================================

# Set the current text and cell color
def SetColor(color): SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color) 

# Read in arrow key input for menu navigation
def GetNormalModeMenuInput(c):
    # Source - https://stackoverflow.com/a/61163868
    # Posted by Adrian Rosoga
    # Retrieved 2026-06-20, License - CC BY-SA 4.0
    with keyboard.Events() as events:
        # Block for as much as possible before returning the key
        return events.get(1e6).key


def SetFlags(flags):
    PROGRAM_FLAGS["SELECTION"] = flags[0]
    PROGRAM_FLAGS["MODE"] = flags[1]
    PROGRAM_FLAGS["STEP"] = flags[2]
    PROGRAM_FLAGS["SELECTED"] = flags[3]
    
    return None

# --- Tokenizer ---
# Handles square brackets [Variable Input] as single tokens.
# Removes select keywords (A, NEW, BUT, EXCEPT, AS, EXACTLY).
vector<string> TokenizeAndClean(const string& input):
    vector<string> tokens
    string currentToken
    bool insideBrackets = false

    # Set of words to ignore completely based on flowchart X's
    set<string> ignoredWords =:
        "NEW", "BUT", "EXCEPT", "AS", "EXACTLY", "A", "NAMED", "CALLED", "TITLED"
    

    for (size_t i = 0 i < input.length() ++i):
        char c = input[i]

        if (c == '['):
            if (!currentToken.empty()):
                tokens.push_back(currentToken)
                currentToken.clear()
            
            insideBrackets = true
            currentToken += c
        
        else if (c == ']'):
            currentToken += c
            insideBrackets = false
            tokens.push_back(currentToken)
            currentToken.clear()
        
        else if (isspace(c) && !insideBrackets):
            if (!currentToken.empty()):
                tokens.push_back(currentToken)
                currentToken.clear()
            
        
        else:
            # Handle period at end of sentence as distinct token if needed, 
            # or just treat as part of word.
            if (c == '.' && !insideBrackets):
                if (!currentToken.empty()):
                    tokens.push_back(currentToken)
                    currentToken.clear()
                
                tokens.push_back(".")
            
            else:
                currentToken += c
            
        
    
    if (!currentToken.empty()) tokens.push_back(currentToken)

    # Filter phase
    vector<string> filteredTokens
    for (const auto& t : tokens):
        # Keep bracketed variables
        if (t.front() == '[' && t.back() == ']'):
            filteredTokens.push_back(t)
        
        else:
            # It's a keyword
            if (ignoredWords.find(t) == ignoredWords.end()):
                filteredTokens.push_back(t)
            
        
    

    return filteredTokens


# Is token a bracketed input?
bool IsVar(const string& token):
    return token.size() >= 2 && token.front() == '[' && token.back() == ']'


# Is token in specified keyword set?
bool IsInSet(const string& token, const set<string>& keywords):
    return keywords.find(token) != keywords.end()


# Strip brackets for final output
string CleanVar(string token):
    if (IsVar(token)):
        return token.substr(1, token.size() - 2)
    
    return token


# Load cures our dementia
CustomSheet load():
    return *(new CustomSheet("nada", "nada"))


# Clears console window
void ClearConsole():
    


# Save lets us store stuff in files
# Felt like being unique and separated everything with pipes
# *.psv = Pipe Separated Values
void save():
    return None

# ==================================================================================================
# =                                          CREATION                                              =
# ==================================================================================================

# Check if the character sheet already exists before it's created again
bool CheckForAlreadyExists(string sName, map<string, CustomSheet>& cSheets):
    if (cSheets.find(sName) != cSheets.end()):
        SetColor(12) cout << "\nERROR: SHEET "
        SetColor(9) cout << "["
        SetColor(15) cout << sName
        SetColor(9) cout << "] "
        SetColor(12) cout << "ALREADY EXISTS!\n\n"
        return true
    
    return false


# Printing Function to make code shorter
void CreatedSheet(string sName, string cName):
    SetColor(14) cout << "\nCREATED "
    SetColor(13) cout << "SHEET "
    SetColor(9) cout << "["
    SetColor(15) cout << sName
    SetColor(9) cout << "] "
    SetColor(12) cout << "WITH "
    SetColor(11) cout << "CHARACTER "
    SetColor(9) cout << "["
    SetColor(15) cout << cName
    SetColor(9) cout << "]\n\n"


# Parses through creation instruction to construct a new character sheet
# Handles filler words like "A", "NEW", and others
# Several optional fields like 'with character' and 'with fields'
string HandleCreate(string inputString, map<string, CustomSheet>& customSheets, bool mode):

    vector<string> inst # Create pointer for tokenized instructions without reserving the space
	SetFlags(new bool[4]{ 0, 0, 0, 0 }) # Reset program flags

    # MENU DISPLAY
    switch (mode):
    case 0:: # normal person mode
        
        # GUI
        string displayMessage = ""

        char c = 0 # Input storage

        while (!PROGRAM_FLAGS.SELECTED):
            cout << CLEAR

            # PUT DECORATIONS HERE LATER
            
            # debug info
            cout << "selection: " << PROGRAM_FLAGS.SELECTION << "\n"
			cout << "mode: " << PROGRAM_FLAGS.MODE << "\n"
			cout << "step: " << PROGRAM_FLAGS.STEP << "\n"
			cout << "selected: " << PROGRAM_FLAGS.SELECTED << "\n"

            displayMessage = "\nWhat do you want to create?"
            displayMessage += "\n---------------------------"

            switch (PROGRAM_FLAGS.SELECTION):
            case 0b00::
                displayMessage += "\nCharacter Sheet File      >"
                displayMessage += "\nCharacter Sheet Field      "
                displayMessage += "\nNew Enemy Type File        "
                displayMessage += "\nNew Enemy Instance File    "
                break
            
            case 0b01::
                displayMessage += "\nCharacter Sheet File       "
                displayMessage += "\nCharacter Sheet Field     >"
                displayMessage += "\nNew Enemy Type File        "
                displayMessage += "\nNew Enemy Instance File    "
                break
            
            case 0b10::
                displayMessage += "\nCharacter Sheet File       "
                displayMessage += "\nCharacter Sheet Field      "
                displayMessage += "\nNew Enemy Type File       >"
                displayMessage += "\nNew Enemy Instance File    "
                break
            
            case 0b11::
                displayMessage += "\nCharacter Sheet File       "
                displayMessage += "\nCharacter Sheet Field      "
                displayMessage += "\nNew Enemy Type File        "
                displayMessage += "\nNew Enemy Instance File   >"
                break
            
            default: break
            

			cout << displayMessage

            # Navigate start menu
            GetNormalModeMenuInput(c)
            PROGRAM_FLAGS.SELECTION = (c == DOWN) ? ++PROGRAM_FLAGS.SELECTION : ((c == UP) ? --PROGRAM_FLAGS.SELECTION : PROGRAM_FLAGS.SELECTION)
            PROGRAM_FLAGS.SELECTED = (c == RIGHT) ? 1 : 0
        

        # After pressing enter/return, move into the sub-menu for the selected option.
        # This sets the current sub-menu to the same as the menu selection from earlier.
        PROGRAM_FLAGS.MODE = PROGRAM_FLAGS.SELECTION
        PROGRAM_FLAGS.STEP++


        return "s"

        break
    
    case 1: # nerd mode
        if (inputString.empty())
            return GetErrors(2) + "\nEXPECTED INPUT"

        # Convert Global Input to Uppercase
        inputString = str(inputString).upper()
        # tokenize instruction data
        inst = TokenizeAndClean(inputString)

        # If you're in nerd mode, the create instruction takes options like -cs, -cf, -et, and -ei corresponding to the options above
        # "CREATE MODE"
        # > 

        return "s"
        break
    



# ==================================================================================================
# =                                          DELETION                                              =
# ==================================================================================================

string HandleDelete(string inputString, map<string, CustomSheet>& customSheets):

    return "s"


# ==================================================================================================
# =                                         FRONT DESK                                             =
# ==================================================================================================

# Parses through input text
# Routes instructions to each department
string HandleInput(string inputString, map<string, CustomSheet>& customSheets):
	# skeleton code while I implement creation functionality
    inputString = str(inputString).upper()

    if (inputString == "CREATE"):
        return HandleCreate(inputString, customSheets, 0) + "\n"
    
    
    ## cat stepped on the keyboard
    #if (inputString.empty()) return GetErrors(2)

    ## Convert all text to uppercase
    #transform(inputString.begin(), inputString.end(), inputString.begin(), toupper)

    ## Parse string into tokens
    #stringstream ss(inputString)
    #string segment
    #vector<string> tokens
    #while (ss >> segment)
    #    tokens.push_back(segment)

    ## Record first and second tokens for use later
    #string first = tokens[0]
    #string second = (tokens.size() > 1) ? tokens[1] : ""

    ## get lost you disgusting vectors...
    #if (!tokens.size() > 0) tokens.erase(tokens.begin())
    #
    ## Use the stupid gross disgusting useless tokens to remove the first keyword from input
    #string restOfInput
    #for (string s : tokens)
    #    restOfInput += s + " "

    ## Check Second Keyword Constraint
    #set<string> restrictedSecond = {
    #    "CREATE", "ADD", "INSERT", "BUILD", "FORM", "NEW"
    #    "REMOVE", "DELETE",
    #    "UPDATE", "CHANGE", "MODIFY", "SET",
    #    "READ", "RETURN", "GET"
    #}

    ## If second keyword IS NOT in the list (or doesn't exist), proceed
    #if (restrictedSecond.find(second) == restrictedSecond.end()):

    #    # Dispatch based on First Keyword
    #    if (IsInSet(first,{ "CREATE", "ADD", "INSERT", "BUILD", "FORM", "NEW" })):
    #        return HandleCreate(restOfInput, customSheets, 0) + "\n"
    #    
    #    else if (IsInSet(first,{ "REMOVE", "DELETE" })):
    #        return HandleDelete(restOfInput, customSheets)
    #    
    #    else if (IsInSet(first,{ "UPDATE", "CHANGE", "MODIFY", "SET" })):
    #        #return HandleUpdate(restOfInput, customSheets)
    #    
    #    else if (IsInSet(first,{ "READ", "RETURN", "GET" })):
    #        #return HandleGet(restOfInput, customSheets)
    #    
    #

    ## stoopid idiots didn't read the documentation
    #return GetErrors(2)


# Retrieve an error given a specified code
def GetErrors(i):
    # change this stupid fucking vector to a fixed array later for optimization
    vector<string> e = {
        "ERROR CODE -1: FIELD DOES NOT EXIST\n",
        "ERROR CODE -2: EXPECTED field OR value FOR UPDATE TYPE\n",
        "ERROR CODE -3: BAD SYNTAX\n" 
        "ERROR CODE -4: SECRET FOUND\n" }
    return e[i]