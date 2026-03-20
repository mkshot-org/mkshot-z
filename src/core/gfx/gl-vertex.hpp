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

#include "core/gfx/etc-internal.hpp"
#include "core/gfx/gl-fun.hpp"
#include "core/gfx/gl-shader.hpp"

/* Simple Vertex */
struct SVertex
{
	Vec2 pos;
	Vec2 texPos;
};

/* Color Vertex */
struct CVertex
{
	Vec2 pos;
	Vec4 color;

	CVertex();
};

struct Vertex
{
	Vec2 pos;
	Vec2 texPos;
	Vec4 color;

	Vertex();
};

struct VertexAttribute
{
	Shader::Attribute index;
	GLint size;
	GLenum type;
	const GLvoid *offset;
};

template<class VertType>
struct VertexTraits
{
	static const VertexAttribute *attr;
	static const GLsizei attrCount;
};


