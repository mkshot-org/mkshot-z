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


#include "util/intru-list.hpp"
#include "core/audio/al-util.hpp"
#include "util/boost-hash.hpp"

#include <string>
#include <vector>

struct SoundBuffer;
struct Config;

struct SoundEmitter
{
	typedef BoostHash<std::string, SoundBuffer*> BufferHash;

	IntruList<SoundBuffer> buffers;
	BufferHash bufferHash;

	/* Byte count sum of all cached / playing buffers */
	uint32_t bufferBytes;

	const size_t srcCount;
	std::vector<AL::Source::ID> alSrcs;
	std::vector<SoundBuffer*> atchBufs;

	/* Indices of sources, sorted by priority (lowest first) */
	std::vector<size_t> srcPrio;

	SoundEmitter(const Config &conf);
	~SoundEmitter();

	void play(const std::string &filename,
	          int volume,
	          int pitch);

	void stop();

private:
	SoundBuffer *allocateBuffer(const std::string &filename);
};


