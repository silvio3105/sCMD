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
 * Simple Command Handler
 * @{
*/


// ----- ENUMATORS
/**
 * @brief Enumator for delimiter types.
 * 
 */
enum sCMD_delimiter_t : uint8_t {
	sCMD_CP = 0, /**< @brief Delimiter between command and parameters. */
	sCMD_PP = 1 /**< @brief Delimiter between parameters. */
};


// ----- TYPEDEFS
/**
 * @brief Typedef for command handler.
 * 
 * @param args Pointer to argument data.
 * @param argCnt Number of received arguments/length of \c args
 * @return No return value.
 */
typedef void (*cmdH)(sStd::Data<char>* args, uint8_t argCnt);


// ----- STRUCTS
/**
 * @brief Struct for command.
 * 
 */
struct CMDList {
	char* cmd = nullptr;
	cmdH cmdHandler = nullptr;
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
	/**
	 * @brief CMD handler constructor.
	 * 
	 * @param list Pointer to list of commands.
	 * @param len Length of \c list
	 * @param cpDelimiter Delimiter character between command and parameters.
	 * @param ppDelimiter Delimiter between parameters.
	 * @return No return value.
	 */
	CMD(CMDList* list, uint16_t len, char cpDelimiter, char ppDelimiter);

	/**
	 * @brief CMD handler deconstructor
	 * 
	 * @return No return value.
	 */
	~CMD(void);


	// METHOD DECLARATIONS
	void parse(char* input);
	void setDelimiter(sCMD_delimiter_t type, char delimiter);
	void getDelimiter(sCMD_delimiter_t type) const;



	// PRIVATE STUFF
	private:
	CMDList* cmdList = nullptr; /**< @brief Pointer to external list of commands.  */
	uint16_t cmdListLen = 0; /**< @brief Length of \c cmdList */
	sStd::Data<char> args[max]; /**< @brief Data object for command's arguments. */
	uint8_t maxArgs = max; /**< @brief Length of \c args array. */
	char delimiter[2] = { '\0', '\0' }; /**< @brief Array for delimiters. */
};


/** @}*/

#endif // _SCMD_H_

// END WITH NEW LINE
