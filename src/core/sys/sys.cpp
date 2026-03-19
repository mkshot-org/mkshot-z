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

#include "core/sys/sys.hpp"

#include <string>
#include <cstring>

#ifdef __WIN32__
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef SECURITY_WIN32
#define SECURITY_WIN32
#endif
#include <windows.h>
#include <security.h>
#else
#include <unistd.h>
#include <pwd.h>
#endif

#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_loadso.h>

#ifdef __WIN32__
static std::string wideToUTF8(const wchar_t *wcStr)
{
	std::string ret;
	int size = WideCharToMultiByte(CP_UTF8, 0, wcStr, -1, 0, 0, 0, 0);
	if (size > 0) {
		char *str = new char[size];
		if (WideCharToMultiByte(CP_UTF8, 0, wcStr, -1, str, size, 0, 0) == size)
			ret = str;
		delete [] str;
	}
	return ret;
}
#endif

std::string mkshot_sys::getLanguage()
{
	std::string ret;

#ifdef __WIN32__
	wchar_t wcBuf[18] = {'\0'};

	wchar_t wcLang[9];
	wchar_t wcCountry[9];

	GetLocaleInfoW(LOCALE_USER_DEFAULT, LOCALE_SISO639LANGNAME, wcLang, sizeof(wcLang) / sizeof(wchar_t));
	GetLocaleInfoW(LOCALE_USER_DEFAULT, LOCALE_SISO3166CTRYNAME, wcCountry, sizeof(wcCountry) / sizeof(wchar_t));

	swprintf(wcBuf, sizeof(wcBuf), L"%ls_%ls", wcLang, wcCountry);

	ret = wideToUTF8(wcBuf);
#else
	const char *locFallback = "en_US";
	const char *loc;

	loc = SDL_getenv("LANG");

	if (!loc || (loc && loc[0] == '\0'))
		loc = SDL_getenv("LC_MESSAGES");

	if (!loc || (loc && loc[0] == '\0'))
		loc = SDL_getenv("LC_ALL");

	if (loc && loc[0] != '\0') {
		std::string tmpLoc(loc);
		std::string::size_type tmpPos;

		// Chomp next XPG part after dot (codeset and modifier)
		tmpPos = tmpLoc.find('.');
		if (tmpPos != std::string::npos)
			tmpLoc.resize(tmpPos);

		if (tmpLoc == "C" || tmpLoc == "POSIX")
			ret = locFallback;
		else
			ret = tmpLoc;
	} else {
		ret = locFallback;
	}
#endif

	return ret;
}

std::string mkshot_sys::getUserName()
{
	std::string ret;

#ifdef __WIN32__
	DWORD size = 0;
	GetUserNameW(0, &size);
	if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
		wchar_t *wcBuf = new wchar_t[size];
		GetUserNameW(wcBuf, &size);
		ret = wideToUTF8(wcBuf);
		delete [] wcBuf;
	}
#else
	uid_t userId;

#ifdef __APPLE__
	userId = geteuid();
#else
	userId = getuid();
#endif

	struct passwd *pw = getpwuid(userId);

	if (pw && (pw->pw_name && pw->pw_name[0] != '\0'))
		ret = pw->pw_name;
#endif

	if (ret.empty()) {
#ifdef __WIN32__
		const char *user = SDL_getenv("USERNAME");
#else
		const char *user = SDL_getenv("USER");
#endif

		if (user && user[0] != '\0')
			ret = user;
		else
			ret = "noname";
	}

	return ret;
}

std::string mkshot_sys::getUserFullName()
{
	std::string ret;

#ifdef __WIN32__
	ULONG size = 0;
	GetUserNameExW(NameDisplay, 0, &size);
	if (GetLastError() == ERROR_MORE_DATA) {
		wchar_t *wcBuf = new wchar_t[size];
		GetUserNameExW(NameDisplay, wcBuf, &size);
		ret = wideToUTF8(wcBuf);
		delete [] wcBuf;
	}
#else
	uid_t userId;

#ifdef __APPLE__
	userId = geteuid();
#else
	userId = getuid();
#endif

	struct passwd *pw = getpwuid(userId);

	if (pw && (pw->pw_gecos && pw->pw_gecos[0] != '\0' && pw->pw_gecos[0] != ',')) {
		std::string tmpGecos(pw->pw_gecos);
		std::string::size_type tmpPos;

		// Chomp next GECOS part after comma,
		// we only need to get only the full name
		tmpPos = tmpGecos.find(',');
		if (tmpPos != std::string::npos)
			tmpGecos.resize(tmpPos);

		ret = tmpGecos;
	}
#endif

	// Fallback to login name if we couldn't get the full name
	if (ret.empty())
		ret = getUserName();

	return ret;
}

int mkshot_sys::getScalingFactor()
{
	// HiDPI scaling not supported outside of macOS for now
	return 1;
}

bool mkshot_sys::isWine()
{
#ifdef __WIN32__
	void *ntdll = SDL_LoadObject("ntdll.dll");
	return SDL_LoadFunction(ntdll, "wine_get_host_version") != 0;
#else
	// Always false on non-Windows builds
	return false;
#endif
}

bool mkshot_sys::isRosetta()
{
	// Always false on non-Mac environment, see impl-apple.mm
	return false;
}

mkshot_sys::WineHostType mkshot_sys::getRealHostType()
{
#ifdef __WIN32__
	void *ntdll = SDL_LoadObject("ntdll.dll");

	void (*wine_get_host_version)(const char **, const char **) =
	(void (*)(const char **, const char **))SDL_LoadFunction(ntdll, "wine_get_host_version");

	if (wine_get_host_version == 0)
		return WineHostType::Windows;

	const char *kernel = 0;
	wine_get_host_version(&kernel, 0);

	if (!strcmp(kernel, "Darwin"))
		return WineHostType::Mac;

	return WineHostType::Linux;
#else
	// Always Linux host type on builds for Linux
	return WineHostType::Linux;
#endif
}
