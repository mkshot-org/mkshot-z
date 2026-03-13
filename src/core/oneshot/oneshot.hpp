/*
** This file is part of ModShot-mkxp-z, further modified for mkshot-z.
**
** Copyright (C) 2026 mkshot-z contributors <https://github.com/mkshot-org>
** Copyright (C) 2024 hat_kid <https://github.com/thehatkid>
**
** ModShot-mkxp-z is licensed under GPLv2 or later.
** mkshot-z is licensed under GPLv3-or-later.
*/

#pragma once

#include "core/etc/internal.hpp"

#include <string>

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
