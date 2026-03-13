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


#include "sigslot.hpp"

#define shState SharedState::instance
#define glState shState->_glState()
#define rgssVer SharedState::rgssVersion

struct SharedStatePrivate;
struct RGSSThreadData;
struct GlobalIBO;
struct SDL_Window;
struct TEXFBO;
struct Quad;
struct ShaderSet;

class Scene;
class FileSystem;
class EventThread;
class Graphics;
class Input;
class Audio;
class Oneshot;
#ifdef MKSHOT_STEAM
class Steam;
#endif
class GLState;
class TexPool;
class Font;
class SharedFontState;
struct GlobalIBO;
struct Config;
struct Vec2i;

struct SharedState
{
	void *bindingData() const;
	void setBindingData(void *data);

	SDL_Window *sdlWindow() const;

	Scene *screen() const;
	void setScreen(Scene &screen);

	FileSystem &fileSystem() const;

	EventThread &eThread() const;
	RGSSThreadData &rtData() const;
	Config &config() const;

	Graphics &graphics() const;
	Input &input() const;
	Audio &audio() const;

	Oneshot &oneshot() const;

#ifdef MKSHOT_STEAM
	Steam &steam() const;
#endif

	GLState &_glState() const;

	ShaderSet &shaders() const;

	TexPool &texPool() const;

	SharedFontState &fontState() const;
	Font &defaultFont() const;

	sigslot::signal<> prepareDraw;

	unsigned int genTimeStamp();
    
    // Returns time since SharedState was constructed in microseconds
    double runTime();

	/* Returns global quad IBO, and ensures it has indices
	 * for at least minSize quads */
	void ensureQuadIBO(size_t minSize);
	GlobalIBO &globalIBO();

	/* Global general purpose texture */
	void bindTex();
	void ensureTexSize(int minW, int minH, Vec2i &currentSizeOut);

	TEXFBO &gpTexFBO(int minW, int minH);

	Quad &gpQuad() const;

	/* Basically just a simple "TexPool"
	 * replacement for Tilemap atlas use */
	void requestAtlasTex(int w, int h, TEXFBO &out);
	void releaseAtlasTex(TEXFBO &tex);

	/* Checks EventThread's shutdown request flag and if set,
	 * requests the binding to terminate. In this case, this
	 * function will most likely not return */
	void checkShutdown();

	void checkReset();

	static SharedState *instance;
	static int rgssVersion;

	/* This function will throw an Exception instance
	 * on initialization error */
	static void initInstance(RGSSThreadData *threadData);
	static void finiInstance();

private:
	SharedState(RGSSThreadData *threadData);
	~SharedState();

	SharedStatePrivate *p;
};


