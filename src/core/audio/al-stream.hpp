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


#include "core/audio/al-util.hpp"
#include "util/sdl.hpp"

#include <string>
#include <SDL_rwops.h>

struct ALDataSource;

#define STREAM_BUFS 3

/* State-machine like audio playback stream.
 * This class is NOT thread safe */
struct ALStream
{
	enum State
	{
		Closed,
		Stopped,
		Playing,
		Paused
	};

	bool looped;
	State state;

	ALDataSource *source;
	SDL_Thread *thread;

	std::string threadName;

	SDL_mutex *pauseMut;
	bool preemptPause;

	/* When this flag isn't set and alSrc is
	 * in 'STOPPED' state, stream isn't over
	 * (it just hasn't started yet) */
	AtomicFlag streamInited;
	AtomicFlag sourceExhausted;

	AtomicFlag threadTermReq;

	AtomicFlag needsRewind;
	float startOffset;

	float pitch;

	AL::Source::ID alSrc;
	AL::Buffer::ID alBuf[STREAM_BUFS];

	uint64_t procFrames;
	AL::Buffer::ID lastBuf;

	SDL_RWops srcOps;

	struct
	{
		ALenum format;
		ALsizei freq;
	} stream;

	enum LoopMode
	{
		Looped,
		NotLooped
	};

	ALStream(LoopMode loopMode,
	         const std::string &threadId);
	~ALStream();

	void close();
	void open(const std::string &filename);
	void stop();
	void play(float offset = 0);
	void pause();

	void setVolume(float value);
	void setPitch(float value);
	State queryState();
	float queryOffset();
	bool queryNativePitch();

private:
	void closeSource();
	void openSource(const std::string &filename);

	void stopStream();
	void startStream(float offset);
	void pauseStream();
	void resumeStream();

	void checkStopped();

	/* thread func */
	void streamData();
};


