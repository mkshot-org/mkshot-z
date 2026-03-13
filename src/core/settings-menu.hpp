/*
** This file is part of mkxp, further modified for mkshot-z.
**
** Copyright (C) 2026 mkshot-z contributors <https://github.com/mkshot-org>
** Copyright (C) 2014 - 2021 Amaryllis Kulla <ancurio@mapleshrine.eu>
**
** mkxp is licensed under GPLv2 or later.
** mkshot-z is licensed under GPLv3 or later.
*/

#pragma once


#include <stdint.h>

struct SettingsMenuPrivate;
struct RGSSThreadData;
union SDL_Event;

class SettingsMenu
{
public:
	SettingsMenu(RGSSThreadData &rtData);
	~SettingsMenu();

	/* Returns true if the event was consumed */
	bool onEvent(const SDL_Event &event);
	void raise();
	bool destroyReq() const;

private:
	SettingsMenuPrivate *p;
};


