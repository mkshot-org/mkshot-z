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

#include <stack>
#include <assert.h>

struct Config;

template<typename T>
struct GLProperty
{
	~GLProperty()
	{
		assert(stack.size() == 0);
	}

	void init(const T &value)
	{
		current = value;
		apply(value);
	}

	void push() { stack.push(current); }
	void pop()  { set(stack.top()); stack.pop(); }
	const T &get()    { return current; }
	void set(const T &value)
	{
		if (value == current)
			return;

		init(value);
	}

	void pushSet(const T &value)
	{
		push();
		set(value);
	}

	void refresh()
	{
		apply(current);
	}
private:
	virtual void apply(const T &value) = 0;

	T current;
	std::stack<T> stack;
};


class GLClearColor : public GLProperty<Vec4>
{
	void apply(const Vec4 &);
};

class GLScissorBox : public GLProperty<IntRect>
{
public:
	/* Sets the intersection of the current box with value */
	void setIntersect(const IntRect &value);

private:
	void apply(const IntRect &value);
};

class GLScissorTest : public GLProperty<bool>
{
	void apply(const bool &value);
};

class GLBlendMode : public GLProperty<BlendType>
{
	void apply(const BlendType &value);
};

class GLBlend : public GLProperty<bool>
{
	void apply(const bool &value);
};

class GLViewport : public GLProperty<IntRect>
{
	void apply(const IntRect &value);
};

class GLProgram : public GLProperty<unsigned int> /* GLuint */
{
	void apply(const unsigned int &value);
};


class GLState
{
public:
	GLClearColor clearColor;
	GLScissorBox scissorBox;
	GLScissorTest scissorTest;
	GLBlendMode blendMode;
	GLBlend blend;
	GLViewport viewport;
	GLProgram program;

	struct Caps
	{
		int maxTexSize;

		Caps();

	} caps;

	GLState(const Config &conf);
};


