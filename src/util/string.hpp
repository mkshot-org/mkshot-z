/*
** mkshot-z - Experimental OneShot (2016) engine reimplementation for modders.
**
** Copyright (C) 2026 Team Reverium <https://github.com/reverium>
** Copyright (C) 2024 hat_kid <https://github.com/thehatkid> (ModShot-mkxp-z)
** Copyright (C) 2013-2023 Amaryllis Kulla and mkxp-z contributors
**
** This program is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
** GNU General Public License for more details.
*/

#pragma once

#ifndef __APPLE__

#include <string>

// Copies the given text with additional newlines every X characters.
// Saves the output into a new std::string object.
// Newlines are only inserted on spaces (' ') or tabs ('\t').
static std::string copyWithNewlines(const char *input, const unsigned limit)
{
    std::string output;
	unsigned noNewlineCount = 0;
	
	while (*input != '\0')
	{
		if ((*input == ' ' || *input == '\t') && noNewlineCount >= limit)
		{
			output += '\n';
			noNewlineCount = 0;
		}
		else 
		{
			output += *input;
			
			if (*input == '\n')
				noNewlineCount = 0;
			else
				noNewlineCount++;
		}

		input++;
	}
    return output;
}

/*
static std::string copyWithNewlines(const std::string& input, const unsigned limit)
{
	return copyWithNewlines(input.c_str(), limit);
}
*/

#endif


