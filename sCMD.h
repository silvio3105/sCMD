/**
 * @file sCMD.h
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief Simple Command Handler header file.
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


#ifndef _SCMD_H_
#define _SCMD_H_

// ----- INCLUDE FILES
#include            "sStd.h"

/** \addtogroup sCMD 
 * Simple Command Handler.
 * Create command list with \ref CMDList struct and command handler object using \ref CMD<max> class template. \c max is maximum number of arguments that command handler can accept.
 * Command handler supports multiple command delimiters(coomand to command, command to arguments and argument to argument). Search for command is case sensitive(cmd != Cmd).
 * See examples for more info.
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
typedef void (*cmdH)(const char* args, const uint8_t argCnt);


// ----- STRUCTS
/**
 * @brief Struct for command.
 * 
 */
struct CMDList {
	const char* cmd; /**< @brief Command C-string. */
	const cmdH cmdHandler; /**< @brief Pointer to command function. See \ref cmdH */
};


// ----- CLASSES
template <uint8_t max>
/**
 * @brief Class for command hanlder.
 * 
 */
class CMD {
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
	CMD(const CMDList* list, uint16_t len, cmdH fallback,  char ccDelimiter = ';', char caDelimiter = ',', char aaDelimiter = ',');

	/**
	 * @brief CMD handler deconstructor.
	 * 
	 * @return No return value.
	 */
	~CMD(void);

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
	uint8_t exe(char* input, uint8_t nullAsArg = 0);

	/**
	 * @brief Set delimiter character.
	 * 
	 * @param type Delimiter type.
	 * @param del Delimiter character.
	 * @return No return value.
	 */
	inline void setDelimiter(uint8_t type, char del);

	/**
	 * @brief Get delimiter character.
	 * 
	 * @param type Delimiter type.
	 * @return Configured delimiter.
	 */
	inline char getDelimiter(uint8_t type) const;


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


// ----- STATIC FUNCTION DECLARATIONS
/**
 * @brief Find \c input command in \c cmdList
 * 
 * @param input Pointer to command C-string.
 * @param cmdList Pointer to external command list.
 * @param len Length of \c cmdList
 * @return \c -1 if command is not found.
 * @return Index of command in \c cmdList if command is found.
 */
static int16_t findCmd(const char* input, const CMDList* cmdList, const uint16_t len);


/** @}*/

#endif // _SCMD_H_

// END WITH NEW LINE
