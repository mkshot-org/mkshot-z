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

#include "core/gfx/vertex.hpp"
#include "core/gfx/util.hpp"
#include "core/gfx/meta.hpp"
#include "core/shared-state.hpp"
#include "core/gfx/global-ibo.hpp"
#include "core/gfx/shader.hpp"

#include <vector>
#include <stdint.h>

template<class VertexType>
struct QuadArray
{
	std::vector<VertexType> vertices;

	VBO::ID vbo;
	GLMeta::VAO vao;

	size_t quadCount;
	GLsizeiptr vboSize;

	QuadArray()
	    : quadCount(0),
	      vboSize(-1)
	{
		vbo = VBO::gen();

		GLMeta::vaoFillInVertexData<VertexType>(vao);
		vao.vbo = vbo;
		vao.ibo = shState->globalIBO().ibo;

		GLMeta::vaoInit(vao);
	}

	~QuadArray()
	{
		GLMeta::vaoFini(vao);
		VBO::del(vbo);
	}

	void resize(size_t size)
	{
		vertices.resize(size * 4);
		quadCount = size;
	}

	void clear()
	{
		vertices.clear();
		quadCount = 0;
	}

	/* This needs to be called after the final 'append()' call
	 * and previous to the first 'draw()' call. */
	void commit()
	{
		VBO::bind(vbo);

		GLsizeiptr size = vertices.size() * sizeof(VertexType);

		if (size > vboSize)
		{
			/* New data exceeds already allocated size.
			 * Reallocate VBO. */
			VBO::uploadData(size, dataPtr(vertices), GL_DYNAMIC_DRAW);
			vboSize = size;

			shState->ensureQuadIBO(quadCount);
		}
		else
		{
			/* New data fits in allocated size */
			VBO::uploadSubData(0, size, dataPtr(vertices));
		}

		VBO::unbind();
	}

	void draw(size_t offset, size_t count)
	{
		GLMeta::vaoBind(vao);

		const char *_offset = (const char*) 0 + offset * 6 * sizeof(index_t);
		gl.DrawElements(GL_TRIANGLES, count * 6, _GL_INDEX_TYPE, _offset);

		GLMeta::vaoUnbind(vao);
	}

	void draw()
	{
		draw(0, quadCount);
	}

	size_t count() const
	{
		return quadCount;
	}
};

typedef QuadArray<Vertex> ColorQuadArray;
typedef QuadArray<SVertex> SimpleQuadArray;


