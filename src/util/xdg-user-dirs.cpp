/*
** mkshot-z - Experimental OneShot (2016) engine reimplementation for modders.
**
** Copyright (C) 2026 Reverium <https://github.com/reverium>
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

#include <string>
#include <cstring>
#include <fstream>
#include <stdlib.h>

std::string xdgUserDirsLookup(const char *type, const char *fallback)
{
	std::string out;

	std::string configPath;
	std::ifstream config;

	const char *home = getenv("HOME");

	if (!home || home[0] == '\0') {
		if (fallback)
			return std::string(fallback);
		return std::string();
	}

	const char *configHome = getenv("XDG_CONFIG_HOME");

	if (configHome && configHome[0] != '\0')
		configPath = std::string(configHome) + "/user-dirs.dirs";
	else
		configPath = std::string(home) + "/.config/user-dirs.dirs";

	config.open(configPath, std::ios::in);

	if (!config.is_open()) {
		if (fallback)
			return std::string(fallback);
		return std::string();
	}

	std::string line;
	while (std::getline(config, line))
	{
		std::string::size_type pos = 0;
		std::string::size_type posMatch = 0;

		std::string key;
		std::string value;
		std::string dir;

		if (line.size() <= 0)
			continue;

		// Skip whitespace
		while (line[pos] == ' ' || line[pos] == '\t')
			pos++;

		// Skip comment
		if (line[pos] == '#')
			continue;

		// Parse variable key
		posMatch = pos;

		while (line[pos] != ' ' && line[pos] != '\t' && line[pos] != '=')
			pos++;

		key = line.substr(posMatch, pos - posMatch);

		// Get XDG directory type name
		std::string::size_type xdgPrefixPos = key.find("XDG_");
		std::string::size_type xdgSuffixPos = key.rfind("_DIR");

		if (xdgPrefixPos == std::string::npos || xdgSuffixPos == std::string::npos)
			continue;

		if (xdgPrefixPos != 0 && xdgSuffixPos != (key.size() - 4))
			continue;
		else
			dir = key.substr(xdgPrefixPos + 4, xdgSuffixPos - 4);

		if (dir != std::string(type))
			continue;

		// Skip until Equals character
		while (line[pos] != '=')
			pos++;

		pos++;

		// Skip whitespace after Equals character
		while (line[pos] == ' ' || line[pos] == '\t')
			pos++;

		// Expect Quote character
		if (line[pos] == '"')
			pos++;

		// Parse variable value
		posMatch = pos;

		for (char c : line.substr(posMatch))
			if (c == '"')
				break;
			else
				pos++;

		value = line.substr(posMatch, pos - posMatch);

		// Modify backslashes (escapings)
		std::string::size_type strpos = 0;
		while ((strpos = value.find('\\', strpos)) != std::string::npos) {
			value.replace(strpos, 1, "");
			strpos += 1;
		}

		// Replace "$HOME/" with Home path from environment variable
		strpos = value.find("$HOME/");
		if (strpos != std::string::npos)
			value.replace(strpos, 5, std::string(home));

		// Remove trailing slashes
		while (value.rbegin() != value.rend() && (*value.rbegin() == '/' || *value.rbegin() == '\\'))
			value.pop_back();

		out = value;
	}

	config.close();

	return out;
}

std::string xdgUserDirsGet(const char *type)
{
	std::string dir = xdgUserDirsLookup(type, NULL);

	if (!dir.empty())
		return dir;

	const char *home = getenv("HOME");

	if (!home || home[0] == '\0')
		return std::string("/tmp");

	// Special case desktop for historical compatibility
	if (strcmp(type, "DESKTOP") == 0) {
		std::string dirDesktop = std::string(home) + "/Desktop";
		return dirDesktop;
	}

	return std::string(home);
}
