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

#include "core/audio/al-data-source.hpp"
#include "util/exception.hpp"

#include <SDL3_sound/SDL_sound.h>

struct SDLSoundSource : ALDataSource
{
    Sound_Sample *sample;
    SDL_IOStream &src_io;
    uint8_t sampleSize;
    bool looped;

    ALenum alFormat;
    ALsizei alFreq;

    SDLSoundSource(SDL_IOStream &io,
                   const char *ext,
                   uint32_t buf_size,
                   bool looped)
        : src_io(io),
          looped(looped)
    {
        sample = Sound_NewSample(&src_io, ext, 0, buf_size);

        if (!sample)
        {
            SDL_CloseIO(&io);
            throw Exception(Exception::SDLError, "SDL_sound: %s", Sound_GetError());
        }

        sampleSize = formatSampleSize(sample->actual.format);

        alFormat = chooseALFormat(sampleSize, sample->actual.channels);
        alFreq = sample->actual.freq;
    }

    ~SDLSoundSource()
    {
        /* This also closes 'src_io' */
        Sound_FreeSample(sample);
    }

    Status fillBuffer(AL::Buffer::ID alBuffer)
    {
        uint32_t decoded = Sound_Decode(sample);

        if (sample->flags & SOUND_SAMPLEFLAG_EAGAIN)
        {
            /* Try to decode one more time on EAGAIN */
            decoded = Sound_Decode(sample);

            /* Give up */
            if (sample->flags & SOUND_SAMPLEFLAG_EAGAIN)
                return ALDataSource::Error;
        }

        if (sample->flags & SOUND_SAMPLEFLAG_ERROR)
            return ALDataSource::Error;

        AL::Buffer::uploadData(alBuffer, alFormat, sample->buffer, decoded, alFreq);

        if (sample->flags & SOUND_SAMPLEFLAG_EOF)
        {
            if (looped)
            {
                Sound_Rewind(sample);
                return ALDataSource::WrapAround;
            }
            else
            {
                return ALDataSource::EndOfStream;
            }
        }

        return ALDataSource::NoError;
    }

    int sampleFreq()
    {
        return sample->actual.freq;
    }

    void seekToOffset(float seconds)
    {
        if (seconds <= 0)
            Sound_Rewind(sample);
        else
            Sound_Seek(sample, static_cast<uint32_t>(seconds * 1000));
    }

    uint32_t loopStartFrames()
    {
        /* Loops from the beginning of the file */
        return 0;
    }

    bool setPitch(float)
    {
        return false;
    }
};

ALDataSource *createSDLSource(SDL_IOStream &io,
                              const char *ext,
                              uint32_t buf_size,
                              bool looped)
{
    return new SDLSoundSource(io, ext, buf_size, looped);
}
