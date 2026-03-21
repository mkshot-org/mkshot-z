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
#include "util/sdl.hpp"

#include <string>
#include <SDL3/SDL_iostream.h>

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

    SDL_Mutex *pauseMut;
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

    SDL_IOStream *src_io;

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


