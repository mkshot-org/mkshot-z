/*
** This file is part of mkxp, further modified for mkshot-z.
**
** Copyright (C) 2026 mkshot-z contributors <https://github.com/mkshot-org>
** Copyright (C) 2014 - 2021 Amaryllis Kulla <ancurio@mapleshrine.eu>
**
** mkxp is licensed under GPLv2 or later.
** mkshot-z is licensed under GPLv3 or later.
*/

#include "core/gfx/vertex.hpp"
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
