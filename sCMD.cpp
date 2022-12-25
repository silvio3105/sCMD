/**
 * @file sCMD.cpp
 * @author silvio3105 (www.github.com/silvio3105)
 * @brief Simple Command Handler source file.
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


// ----- FUNCTION DEFINITIONS
int16_t findCmd(const char* input, const CMDList* cmdList, const uint16_t len)
{
	for (uint16_t i = 0; i < len; i++)
	{
		// If command is found, return index
		if (sStd::cmp(input, cmdList[i].cmd) == SSTD_OK) return i;
	}

	return -1;
}

// END WITH NEW LINE
