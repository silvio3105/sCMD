/**
 * @file sCMD.cpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief Simple Command Handler source file.
 * @version v0.1
 * @date 13.11.2022
 * 
 * @copyright Copyright (c) 2022, silvio3105
 * 
 */


/*
License

Copyright (c) 2022, silvio3105 (www.github.com/silvio3105)

Access and use of this Project and its contents are granted free of charge to any Person.
The Person is allowed to copy, modify and use The Project and its contents only for non-commercial use.
Commercial use of this Project and its contents is prohibited.
Modifying this License and/or sublicensing is prohibited.

THE PROJECT AND ITS CONTENT ARE PROVIDED "AS IS" WITH ALL FAULTS AND WITHOUT EXPRESSED OR IMPLIED WARRANTY.
THE AUTHOR KEEPS ALL RIGHTS TO CHANGE OR REMOVE THE CONTENTS OF THIS PROJECT WITHOUT PREVIOUS NOTICE.
THE AUTHOR IS NOT RESPONSIBLE FOR DAMAGE OF ANY KIND OR LIABILITY CAUSED BY USING THE CONTENTS OF THIS PROJECT.

This License shall be included in all methodal textual files.
*/


// ----- INCLUDE FILES
#include			"sCMD.h"


// ----- CMD MOTHOD DEFINITIONS
template <uint8_t max>
CMD<max>::CMD(const CMDList* list, uint16_t len, cmdH fallback, char ccDelimiter, char cpDelimiter, char ppDelimiter)
{
	// Set command list
	cmdList = list;
	cmdListLen = len;

	// Set fallback function
	cmdFallback = fallback;

	// Set delimiters
	setDelimiter(sCMD_CC, ccDelimiter);
	setDelimiter(sCMD_CA, cpDelimiter);
	setDelimiter(sCMD_AA, ppDelimiter);
}

template <uint8_t max>
CMD<max>::~CMD(void)
{
	// Reset command list
	cmdList = nullptr;
	cmdListLen = 0;

	// Reset fallback
	cmdFallback = nullptr;

	// Reset delimiters
	setDelimiter(sCMD_CC, '\0');
	setDelimiter(sCMD_CA, '\0');
	setDelimiter(sCMD_AA, '\0');
}


// cmd,param1,...,paramN;\0
// cmd;\0
template <uint8_t max>
uint8_t CMD<max>::exe(char* input, uint8_t nullAsArg)
{
	char* nextCmd = nullptr;
	char* nextArg = nullptr;
	char* currCmd = nullptr;
	char* currArg = nullptr;	
	int16_t cmdIdx = -1;
	uint8_t argCnt = 0;
	uint8_t cmdCnt = 0;

	// Set pointer to first command
	nextCmd = input;

	// While next command is found
	do
	{
		// Reset argument counter
		argCnt = 0;

		// Copy command start address
		currCmd = nextCmd;

		// Find next command
		nextCmd = sStd::tok(nextCmd, delimiter[sCMD_CC]);

		// Find start address of first argument
		nextArg = sStd::tok(currCmd, delimiter[sCMD_CA]);
		
		// If argument part is found
		if (nextArg)
		{
    		// While command to argument delimiter is found
    		do
    		{
    			// Stop parsing arguments if argument limit is reached
    			if (argCnt == maxArgs) break;

				// Copy argument start address
    			currArg = nextArg;
    
    			// Find next argument
    			nextArg = sStd::tok(nextArg, delimiter[sCMD_AA]);    		    
    
				// If current argument is not \0 or if nullAsArg is set to 1
				if (*currArg || nullAsArg)
				{
					// Copy argument's start address to argument array
					args[argCnt] = currArg;
		
					// Increase argument counter
					argCnt++;
				}
    		}
    		while (nextArg);
		}

		// If current command is not \0
        if (*currCmd)
        {
    		// Find command index in command list
    		cmdIdx = findCmd(currCmd, cmdList, cmdListLen);
    
    		// Call fallback handler if command was not found
    		if (cmdIdx == -1) cmdFallback((const char**)currCmd, 0);
    		else // Command is found
			{		
				// Call command arguments
				cmdList[cmdIdx].cmdHandler((const char**)args, argCnt);

				// Increase command counter
				cmdCnt++;
			}   
        }
	}
	while (nextCmd);
	
	// Return number of executed commands
	return cmdCnt;
}

template <uint8_t max>
inline void CMD<max>::setDelimiter(uint8_t type, char del)
{
	// Set delimiter for type
	delimiter[type] = del;
}

template <uint8_t max>
inline char CMD<max>::getDelimiter(uint8_t type) const
{
	// Get delimiter for type
	return delimiter[type];
}


// ----- STATIC FUNCTION DEFINITIONS
static int16_t findCmd(const char* input, const CMDList* cmdList, const uint16_t len)
{
	for (uint16_t i = 0; i < len; i++)
	{
		// If command is found, return index
		if (sStd::cmp(input, cmdList[i].cmd) == SSTD_OK) return i;
	}

	return -1;
}


// END WITH NEW LINE
