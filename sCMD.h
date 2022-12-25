/**
 * @file sCMD.h
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief Simple Command Handler header file.
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


#ifndef _SCMD_H_
#define _SCMD_H_

// ----- INCLUDE FILES
#include            "sStd.h"

/** \addtogroup sCMD 
 * Simple Command Handler.
 * 
 * Command format is: \c cmd1:param1,...,paramN;...;cmdN:param1,...,paramN where \c ; is coomand to command delimiter, \c : command to argument delimiter and \c , argument to argument delimiter. Command can be without arguments.
 * 
 * Create command list with \ref CMDList struct and command handler object using \ref CMD<max> class template. \c max is maximum number of arguments that command handler can accept.
 * Command handler supports multiple command delimiters(coomand to command, command to arguments and argument to argument). Search for command is case sensitive(cmd != Cmd).
 * See examples for beter understanding.
 * @{
*/


// ----- MACRO DEFINITIONS
// DELIMITER TYPES
#define sCMD_CC			0 /**< @brief Delimiter between commands. */
#define sCMD_CA			1 /**< @brief Delimiter between command and arguments. */
#define sCMD_AA			2 /**< @brief Delimiter between arguments. */


// ----- TYPEDEFS
/**
 * @brief Typedef for command handler.
 * 
 * @param args Pointer to arguments.
 * @param argCnt Number of received arguments/length of \c args
 * @return No return value.
 */
typedef void (*cmdH)(const char** args, const uint8_t argCnt);


// ----- STRUCTS
/**
 * @brief Struct for command.
 * 
 */
struct CMDList {
	const char* cmd; /**< @brief Command C-string. */
	const cmdH cmdHandler; /**< @brief Pointer to command function. See \ref cmdH */
};


// ----- FUNCTION DECLARATIONS
/**
 * @brief Find \c input command in \c cmdList
 * 
 * @param input Pointer to command C-string.
 * @param cmdList Pointer to external command list.
 * @param len Length of \c cmdList
 * @return \c -1 if command is not found.
 * @return Index of command in \c cmdList if command is found.
 */
int16_t findCmd(const char* input, const CMDList* cmdList, const uint16_t len);


// ----- CLASSES
/**
 * @brief Class for command hanlder.
 * 
 * @tparam max Maximum number of arguments command handler can handle for one command.
 */
template <uint8_t max>
class CMDHandler {
	// PUBLIC STUFF
	public:
	// CONSTRUCTOR & DECONSTRUCTOR METHOD DECLARATIONS
	/**
	 * @brief CMD handler constructor.
	 * 
	 * @param list Pointer to list of commands.
	 * @param len Length of \c list
	 * @param fallback Pointer to external function that is called when unknown command is found.
	 * @param ccDelimiter Delimiter character between commands. Default delimiter is \c ;
	 * @param caDelimiter Delimiter character between command and arguments. Default delimiter is \c ,
	 * @param aaDelimiter Delimiter character between arguments. Default delimiter is \c ,
	 * @return No return value.
	 */
	CMDHandler(const CMDList* list, uint16_t len, cmdH fallback,  char ccDelimiter = ';', char caDelimiter = ',', char aaDelimiter = ',')
	{
		// Set command list
		cmdList = list;
		cmdListLen = len;

		// Set fallback function
		cmdFallback = fallback;

		// Set delimiters
		setDelimiter(sCMD_CC, ccDelimiter);
		setDelimiter(sCMD_CA, caDelimiter);
		setDelimiter(sCMD_AA, aaDelimiter);		
	}

	/**
	 * @brief CMD handler deconstructor.
	 * 
	 * @return No return value.
	 */
	~CMDHandler(void)
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

	// METHOD DECLARATIONS
	/**
	 * @brief Parse and execute input C-string.
	 * 
	 * @param input Pointer to C-string.
	 * @param nullAsArg Set to \c 1 to include \c \0 as argument. By default, this parameter is set to \c 0
	 * @return Number of executed commands.
	 * 
	 * @warning This function modifies \c input C-string!
	 * @note This function parses input C-string from left to right.
	 */
	uint8_t exe(char* input, uint8_t nullAsArg = 0)
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

	/**
	 * @brief Set delimiter character.
	 * 
	 * @param type Delimiter type.
	 * @param del Delimiter character.
	 * @return No return value.
	 */
	inline void setDelimiter(uint8_t type, char del)
	{
		// Set delimiter for the type
		delimiter[type] = del;
	}

	/**
	 * @brief Get delimiter character.
	 * 
	 * @param type Delimiter type.
	 * @return Configured delimiter.
	 */
	inline char getDelimiter(uint8_t type) const
	{
		// Get delimiter for the type
		return delimiter[type];
	}


	// PRIVATE STUFF
	private:
	// VARIABLES
	/**
	 * @brief Pointer to external fallback function.
	 * 
	 * This function is called when command is not found.
	 * Function parameter \c args points to command C-string that does not exist. See \ref cmdH
	 */
	cmdH cmdFallback = nullptr;	
	const CMDList* cmdList; /**< @brief Pointer to external list of commands.  */
	uint8_t maxArgs = max; /**< @brief Length of \ref args array. */
	char delimiter[3] = { '\0', '\0', '\0' }; /**< @brief Array with CC, CA and AA delimiters. */
	char* args[max]; /**< @brief Pointer to all command's arguments. */	
	uint16_t cmdListLen = 0; /**< @brief Length of \ref cmdList */
};


/** @}*/

#endif // _SCMD_H_

// END WITH NEW LINE
