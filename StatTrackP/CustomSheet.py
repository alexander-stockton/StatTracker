import functions

class CustomSheet:
    def __init__(self, sType, sName, cName):
        self.Type = sType
        self.sheetName = sName
        self.characterName = cName
        self.customFields = []

    def updateCustomField(self, fieldName, newValue): # Update the value of a custom field
        if self.customFields.__len__() == 0:
            print("NO CUSTOM FIELDS TO UPDATE")
        for _ in self.self.customFields:
            if _[0] == fieldName:
                try:
                    _[1] = newValue
                except ValueError:
                    print("VALUE " + newValue + "WRONG TYPE FOR FIELD " + fieldName)
        return None

    def removeAllCustomFields(self): # Remove ALL custom fields from the sheet
        self.customFields = {}
        return None

    def resetAllCustomFieldValues(self): # Set the values in all custom fields to "nada"
        if self.customFields.__len__() == 0:
            print("NO CUSTOM FIELDS TO UPDATE")
        for _ in self.self.customFields:
            _[1] = "nada"
        return None


    # Create a new custom field
    def addCustomField(self, fieldName, fieldValue):
        self.customFields.append([fieldName, fieldValue])

        functions.SetColor(14); print("\nADDED ")
        functions.SetColor(13); print("FIELD ")
        functions.SetColor(9); print("[")
        functions.SetColor(15); print(fieldName)
        functions.SetColor(9); print("] ")
        functions.SetColor(12); print("WITH ")
        functions.SetColor(13); print("VALUE ")
        functions.SetColor(9); print("[")
        functions.SetColor(15); print(fieldValue)
        functions.SetColor(9); print("] ")
        functions.SetColor(12); print("TO ")
        functions.SetColor(11); print("CHARACTER ")
        functions.SetColor(9); print("[")
        functions.SetColor(15); print(self.characterName)
        functions.SetColor(9); print("]\n")

        return None 

    # Remove a custom field
    def removeCustomField(self, fieldName):
        self.customFields.remove(fieldName)
		
        functions.SetColor(14); print("\nREMOVED ")
        functions.SetColor(13); print("FIELD ")
        functions.SetColor(9); print("[")
        functions.SetColor(15); print(fieldName)
        functions.SetColor(9); print("] ")
        functions.SetColor(12); print("FROM ")
        functions.SetColor(11); print("CHARACTER ")
        functions.SetColor(9); print("[")
        functions.SetColor(15); print(self.characterName)
        functions.SetColor(9); print("]\n")
		
        return None


    # Retrieve the value of a custom field
    def getCustomField(self, fieldName):
        try:
            for field in self.customFields:
                if field[0] == fieldName:
                    if (self.characterName.upper()[-1] == 'S' or self.characterName.upper()[-1] == 'Z'):
                        return "\n" + self.characterName + "' " + fieldName + ": " + field[1]
                    else:
                        return "\n" + self.characterName + "'s " + fieldName + ": " + field[1]
            
            print(functions.GetErrors(0))
        
        except ValueError:
            functions.SetColor(12)
            return "VALUE ERROR"

    # Update the value of a custom field
    def updateCustomField(self, Type, fieldName, newValue):
        try:
            for field in self.customFields:
                if field[0] == fieldName:
                    copiedValue = self.customFields[fieldName]

                    if (Type == "FIELD"):
                        self.customFields.erase(fieldName)
                        self.customFields.insert({ newValue, copiedValue })
                        
                        functions.SetColor(14); print("\nCHANGED ")
                        functions.SetColor(11); print("NAME ")
                        functions.SetColor(12); print("OF ")
                        functions.SetColor(13); print("FIELD ")
                        functions.SetColor(9); print("[")
                        functions.SetColor(15); print(fieldName)
                        functions.SetColor(9); print("] ")
                        functions.SetColor(12); print("FOR ")
                        functions.SetColor(11); print("CHARACTER ")
                        functions.SetColor(9); print("[")
                        functions.SetColor(15); print(self.self.characterName)
                        functions.SetColor(9); print("] ")
                        functions.SetColor(12); print("TO ")
                        functions.SetColor(9); print("[")
                        functions.SetColor(15); print(newValue)
                        functions.SetColor(9); print("]\n")
                    elif (Type == "VALUE"):
                        self.customFields[fieldName] = newValue

                        functions.SetColor(14); print("\nCHANGED ")
                        functions.SetColor(13); print("VALUE ")
                        functions.SetColor(12); print("IN ")
                        functions.SetColor(13); print("FIELD ")
                        functions.SetColor(9); print("[")
                        functions.SetColor(15); print(fieldName)
                        functions.SetColor(9); print("] ")
                        functions.SetColor(12); print("FOR ")
                        functions.SetColor(11); print("CHARACTER ")
                        functions.SetColor(9); print("[")
                        functions.SetColor(15); print(self.characterName)
                        functions.SetColor(9); print("] ")
                        functions.SetColor(12); print("FROM ")
                        functions.SetColor(9); print("[")
                        functions.SetColor(15); print(copiedValue)
                        functions.SetColor(9); print("] ")
                        functions.SetColor(12); print("TO ")
                        functions.SetColor(9); print("[")
                        functions.SetColor(15); print(newValue)
                        functions.SetColor(9); print("]\n")
                    else:
                        functions.SetColor(9)
                        functions.GetErrors(1)
                    
            print(functions.GetError(1))
            
        except ValueError:
            functions.SetColor(12)
            print("\nVALUE ERROR\n")
        
        return None

    # [CustomSheet.cpp]

    # Remove ALL custom fields from the map
    def removeAllCustomFields(self):
        if self.customFields.__len__() == 0:
            functions.SetColor(12); print("\nNO FIELDS TO REMOVE FOR [" + self.characterName + "].\n")
            return None
        self.customFields = {} # clear this

        functions.SetColor(14); print("\nREMOVED ")
        functions.SetColor(12); print("ALL " ) # Red for emphasis
        functions.SetColor(13); print("FIELDS ")
        functions.SetColor(12); print("FROM ")
        functions.SetColor(11); print("CHARACTER ")
        functions.SetColor(9); print("[")
        functions.SetColor(15); print(self.characterName)
        functions.SetColor(9); print("]\n")
        
        return None

    # Set ALL custom field values to "nada"
    def resetAllCustomFieldValues(self):
        if self.customFields.__len__() == 0:
            functions.SetColor(12); print("\nNO FIELDS TO RESET FOR [" + self.characterName + "].\n")
            return None

        # Iterate and set values to "nada"
        for _ in self.customFields:
            _[1] = "nada"

        functions.SetColor(14); print("\nRESET ")
        functions.SetColor(12); print("ALL ")
        functions.SetColor(13); print("VALUES ")
        functions.SetColor(12); print("TO ")
        functions.SetColor(9); print("[")
        functions.SetColor(15); print("nada")
        functions.SetColor(9); print("] ")
        functions.SetColor(12); print("FOR ")
        functions.SetColor(11); print("CHARACTER ")
        functions.SetColor(9); print("[")
        functions.SetColor(15); print(self.characterName)
        functions.SetColor(9); print("]\n")
        
        return None