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

#include "core/config.hpp"
#include "core/gfx/gfx.hpp"
#include "core/gfx/vertex.hpp"
#include "core/gfx/util.hpp"
#include "core/gfx/meta.hpp"
#include "core/shared-state.hpp"
#include "core/gfx/global-ibo.hpp"
#include "core/gfx/shader.hpp"

struct Quad
{
	Vertex vert[4];
	VBO::ID vbo;
	GLMeta::VAO vao;
	bool vboDirty;

	template<typename V>
	static void setPosRect(V *vert, const FloatRect &r)
	{
		int i = 0;
		vert[i++].pos = r.topLeft();
		vert[i++].pos = r.topRight();
		vert[i++].pos = r.bottomRight();
		vert[i++].pos = r.bottomLeft();
	}

	template<typename V>
	static void setTexRect(V *vert, const FloatRect &r)
	{
		int i = 0;
		vert[i++].texPos = r.topLeft();
		vert[i++].texPos = r.topRight();
		vert[i++].texPos = r.bottomRight();
		vert[i++].texPos = r.bottomLeft();
	}

	template<typename V>
	static int setTexPosRect(V *vert, const FloatRect &tex, const FloatRect &pos)
	{
		setPosRect(vert, pos);
		setTexRect(vert, tex);

		return 1;
	}

	template<typename V>
	static void setColor(V *vert, const Vec4 &c)
	{
		for (int i = 0; i < 4; ++i)
			vert[i].color = c;
	}

	Quad()
	    : vbo(VBO::gen()),
	      vboDirty(true)
	{
		GLMeta::vaoFillInVertexData<Vertex>(vao);
		vao.vbo = vbo;
		vao.ibo = shState->globalIBO().ibo;

		GLMeta::vaoInit(vao, true);
		VBO::allocEmpty(sizeof(Vertex[4]), GL_DYNAMIC_DRAW);
		GLMeta::vaoUnbind(vao);

		setColor(Vec4(1, 1, 1, 1));
	}

	~Quad()
	{
		GLMeta::vaoFini(vao);
		VBO::del(vbo);
	}

	void updateBuffer()
	{
		VBO::bind(vbo);
		VBO::uploadSubData(0, sizeof(Vertex[4]), vert);
		VBO::unbind();
	}

	void setPosRect(const FloatRect &r)
	{
		setPosRect(vert, r);
		vboDirty = true;
	}

	void setTexRect(const FloatRect &r)
	{
		setTexRect(vert, r);
		vboDirty = true;
	}

	void setTexPosRect(const FloatRect &tex, const FloatRect &pos)
	{
		setTexPosRect(vert, tex, pos);
		vboDirty = true;
	}

	void setColor(const Vec4 &c)
	{
		for (int i = 0; i < 4; ++i)
			vert[i].color = c;

		vboDirty = true;
	}

	void draw()
	{
		if (vboDirty)
		{
			updateBuffer();
			vboDirty = false;
		}

		GLMeta::vaoBind(vao);
		gl.DrawElements(GL_TRIANGLES, 6, _GL_INDEX_TYPE, 0);
		GLMeta::vaoUnbind(vao);
	}
};


