/*
** This file is part of mkxp, further modified for mkshot-z.
**
** Copyright (C) 2026 mkshot-z contributors <https://github.com/mkshot-org>
** Copyright (C) 2013 - 2021 Amaryllis Kulla <ancurio@mapleshrine.eu>
**
** mkxp is licensed under GPLv2 or later.
** mkshot-z is licensed under GPLv3 or later.
*/

#pragma once


#include "util/util.hpp"

/* Concerning the 'pos' parameter:
 *   RGSS3 actually doesn't specify a format for this,
 *   it's only implied that it is a numerical value
 *   (must be 0 on invalid cases), and it's not used for
 *   anything outside passing it back into bgm/bgs_play.
 *   We use this freedom to define pos to be a float,
 *   in seconds of playback. (RGSS3 seems to use large
 *   integers that _look_ like sample offsets but I can't
 *   quite make out their meaning yet) */

struct AudioPrivate;
struct RGSSThreadData;

class Audio
{
public:
	void bgmPlay(const char *filename,
	             int volume = 100,
	             int pitch = 100,
	             float pos = 0,
                 int track = -127);
	void bgmStop(int track = -127);
	void bgmFade(int time, int track = -127);
    int bgmGetVolume(int track = -127);
    void bgmSetVolume(int volume = 100, int track = -127);

	void bgsPlay(const char *filename,
	             int volume = 100,
	             int pitch = 100,
	             float pos = 0);
	void bgsStop();
	void bgsFade(int time);

	void mePlay(const char *filename,
	            int volume = 100,
	            int pitch = 100);
	void meStop();
	void meFade(int time);

	void sePlay(const char *filename,
	            int volume = 100,
	            int pitch = 100);
	void seStop();

	float bgmPos(int track = 0);
	float bgsPos();

	void reset();

	DECL_ATTR(GlobalBGMVolume, int);
	DECL_ATTR(GlobalSFXVolume, int);

private:
	Audio(RGSSThreadData &rtData);
	~Audio();

	friend struct SharedStatePrivate;

	AudioPrivate *p;
};


