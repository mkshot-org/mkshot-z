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

#define MKSHOT_PLATFORM_WINDOWS 0
#define MKSHOT_PLATFORM_MACOS 1
#define MKSHOT_PLATFORM_LINUX 2

#ifdef _WIN32
#define MKSHOT_PLATFORM MKSHOT_PLATFORM_WINDOWS
#elif defined __APPLE__
#define MKSHOT_PLATFORM MKSHOT_PLATFORM_MACOS
#elif defined __linux__
#define MKSHOT_PLATFORM MKSHOT_PLATFORM_LINUX
#else
#error "Can't identify platform."
#endif

#include <string>

namespace systemImpl
{
	enum WineHostType
	{
		Windows,
		Linux,
		Mac
	};

	std::string getLanguage();
	std::string getUserName();
	std::string getUserFullName();

	int getScalingFactor();

	bool isWine();
	bool isRosetta();
	WineHostType getRealHostType();
}

#ifdef __APPLE__
std::string getPlistValue(const char *key);
void openSettingsWindow();
bool isMetalSupported();
#endif

namespace mkshot_sys = systemImpl;
