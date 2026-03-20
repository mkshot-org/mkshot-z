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

#pragma once

#include "core/gfx/etc-internal.hpp"

#include <string>
#include <vector>

struct RGSSThreadData;

struct OneshotPrivate;

class Journal;
class Wallpaper;

class Oneshot
{
private:
	OneshotPrivate *p;
	RGSSThreadData &threadData;

public:
	Oneshot(RGSSThreadData &threadData);
	~Oneshot();

	Journal *journal;
	Wallpaper *wallpaper;

	// Message box types
	enum
	{
		MSG_INFO,
		MSG_YESNO,
		MSG_WARN,
		MSG_ERR
	};

	// Functions
	void update();
	bool msgbox(int type, const char *body, const char *title);

	// Mutators
	void setYesNo(const char *yes, const char *no);
	void setWindowPos(int x, int y);
	void setAllowExit(bool allowExit);
	void setExiting(bool exiting);
	void resetObscured();

	// Accessors
	const std::string &os() const;
	const std::string &lang() const;
	const std::string &userName() const;
	const std::string &savePath() const;
	const std::string &docsPath() const;
	const std::string &gamePath() const;
	const std::string &journalName() const;
	const std::vector<uint8_t> &obscuredMap() const;
	bool obscuredCleared() const;
	bool allowExit() const;
	bool exiting() const;

	// Dirty flag for Obscured texture
	bool obscuredDirty;

	// Linux: Desktop Environment string
	std::string desktopEnv;
};
