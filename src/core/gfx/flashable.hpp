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

#include "core/gfx/etc.hpp"
#include "core/gfx/etc-internal.hpp"

class Flashable
{
public:
	Flashable()
	    : flashColor(0, 0, 0, 0),
	      flashing(false),
	      emptyFlashFlag(false)
	{}

	virtual ~Flashable() {}

	void flash(const Vec4 *color, int duration)
	{
		if (duration < 1)
			return;

		flashing = true;
		this->duration = duration;
		counter = 0;

		if (!color)
		{
			emptyFlashFlag = true;
			return;
		}

		flashColor = *color;
		flashAlpha = flashColor.w;
	}

	virtual void update()
	{
		if (!flashing)
			return;

		if (++counter > duration)
		{
			/* Flash finished. Cleanup */
			flashColor = Vec4(0, 0, 0, 0);
			flashing = false;
			emptyFlashFlag = false;
			return;
		}

		/* No need to update flash color on empty flash */
		if (emptyFlashFlag)
			return;

		float prog = (float) counter / duration;
		flashColor.w = flashAlpha * (1 - prog);
	}

protected:
	Vec4 flashColor;
	bool flashing;
	bool emptyFlashFlag;
private:
	float flashAlpha;
	int duration;
	int counter;
};


