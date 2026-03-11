/*
** wallpaper.hpp
**
** This file is part of ModShot-mkxp-z, further modified for mkshot-z.
**
** Copyright (C) 2026 mkshot-z contributors <https://github.com/mkshot-org>
** Copyright (C) 2024 hat_kid <https://github.com/thehatkid>
**
** ModShot-mkxp-z is licensed under GPLv2 or later.
** mkshot-z is licensed under GPLv3-or-later.
*/

#pragma once

struct WallpaperPrivate;

class Wallpaper
{
private:
	WallpaperPrivate *p;

	void cache();

public:
	Wallpaper();
	~Wallpaper();

	void set(const char *name, int color);
	void reset();
};
