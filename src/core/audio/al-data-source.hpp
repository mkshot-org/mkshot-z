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

#include "core/audio/al-util.hpp"

struct ALDataSource
{
	enum Status
	{
		NoError,
		EndOfStream,
		WrapAround,
		Error
	};

	virtual ~ALDataSource() {}

	/* Read/process next chunk of data, and attach it
	 * to provided AL buffer */
	virtual Status fillBuffer(AL::Buffer::ID alBuffer) = 0;

	virtual int sampleRate() = 0;

	/* If the source doesn't support seeking, it will
	 * reset back to the beginning */
	virtual void seekToOffset(float seconds) = 0;

	/* The frame count right after wrap around */
	virtual uint32_t loopStartFrames() = 0;

	/* Returns false if not supported */
	virtual bool setPitch(float value) = 0;
};

ALDataSource *createSDLSource(SDL_IOStream &io,
                              const char *extension,
			                  uint32_t maxBufSize,
			                  bool looped,
			                  int fallbackMode);

ALDataSource *createVorbisSource(SDL_IOStream &io,
                                 bool looped);


