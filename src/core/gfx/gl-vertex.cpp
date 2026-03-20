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

#include "core/gfx/gl-vertex.hpp"
#include "util/util.hpp"

#include <cstddef>

CVertex::CVertex()
    : color(1, 1, 1, 1)
{}

Vertex::Vertex()
    : color(1, 1, 1, 1)
{}

#define o(type, mem) ((const GLvoid*) offsetof(type, mem))

static const VertexAttribute SVertexAttribs[] =
{
	{ Shader::Position, 2, GL_FLOAT, o(SVertex, pos)    },
	{ Shader::TexCoord, 2, GL_FLOAT, o(SVertex, texPos) }
};

static const VertexAttribute CVertexAttribs[] =
{
	{ Shader::Color,    4, GL_FLOAT, o(CVertex, color) },
	{ Shader::Position, 2, GL_FLOAT, o(CVertex, pos)   }
};

static const VertexAttribute VertexAttribs[] =
{
	{ Shader::Color,    4, GL_FLOAT, o(Vertex, color)  },
	{ Shader::Position, 2, GL_FLOAT, o(Vertex, pos)    },
	{ Shader::TexCoord, 2, GL_FLOAT, o(Vertex, texPos) }
};

#define DEF_TRAITS(VertType) \
	template<> \
	const VertexAttribute *VertexTraits<VertType>::attr = VertType##Attribs; \
	template<> \
	const GLsizei VertexTraits<VertType>::attrCount = ARRAY_SIZE(VertType##Attribs)

DEF_TRAITS(SVertex);
DEF_TRAITS(CVertex);
DEF_TRAITS(Vertex);
