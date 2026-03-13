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
#include "core/audio/al-stream.hpp"
#include "util/sdl.hpp"

#include <string>

struct AudioStream
{
	struct
	{
		std::string filename;
		float volume;
		float pitch;
	} current;

	/* Volumes set by external threads,
	 * such as for fade-in/out.
	 * Multiplied together for final
	 * playback volume. Used with setVolume().
	 * Base is set by play().
     * BaseRatio is set by Audio::setVolume() when no specific channel is specified.
       It should be identical among all BGM tracks.
	 * External is used by MeWatch */
	enum VolumeType
	{
		Base = 0,
        BaseRatio,
		FadeOut,
		FadeIn,
        External,
		VolumeTypeCount
	};

	/* Note that 'extPaused' and 'noResumeStop' are
	 * effectively only used with the AudioStream
	 * instance representing the BGM.
	 * They are not AtomicFlags because they're regarded
	 * as part of the underlying stream state, and
	 * always accessed with the stream lock held */

	/* Flag indicating that the MeWatch paused this
	 * (BGM) stream because a ME started playing.
	 * While this flag is set, calls to 'play()'
	 * might open another file, but will not start
	 * the playback stream (the MeWatch will start
	 * it as soon as the ME finished playing). */
	bool extPaused;

	/* Flag indicating that this stream shouldn't be
	 * started by the MeWatch when it is in stopped
	 * state (eg. because the BGM stream was explicitly
	 * stopped by the user script while the ME was playing.
	 * When a new BGM is started (via 'play()') while an ME
	 * is playing, the file will be loaded without starting
	 * the stream, but we want the MeWatch to start it as
	 * soon as the ME ends, so we unset this flag. */
	bool noResumeStop;

	ALStream stream;
	SDL_mutex *streamMut;

	/* Fade out */
	struct
	{
		/* Fade out is in progress */
		AtomicFlag active;

		/* Request fade thread to finish and
		 * cleanup (like it normally would) */
		AtomicFlag reqFini;

		/* Request fade thread to terminate
		 * immediately */
		AtomicFlag reqTerm;

		SDL_Thread *thread;
		std::string threadName;

		/* Amount of reduced absolute volume
		 * per ms of fade time */
		float msStep;

		/* Ticks at start of fade */
		uint32_t startTicks;
	} fade;

	/* Fade in */
	struct
	{
		AtomicFlag rqFini;
		AtomicFlag rqTerm;

		SDL_Thread *thread;
		std::string threadName;

		uint32_t startTicks;
	} fadeIn;

	AudioStream(ALStream::LoopMode loopMode,
	            const std::string &threadId);
	~AudioStream();

	void play(const std::string &filename,
	          int volume,
	          int pitch,
	          float offset = 0);
	void stop();
	void fadeOut(int duration);
	void seek(float offset);

	/* Any access to this classes 'stream' member,
	 * whether state query or modification, must be
	 * protected by a 'lock'/'unlock' pair */
	void lockStream();
	void unlockStream();

	void setVolume(VolumeType type, float value);
	float getVolume(VolumeType type);

	float playingOffset();

private:
	float volumes[VolumeTypeCount];
	void updateVolume();

	void finiFadeOutInt();
	void startFadeIn();

	void fadeOutThread();
	void fadeInThread();
};


