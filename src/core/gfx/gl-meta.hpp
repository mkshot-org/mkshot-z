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


#include "core/gfx/fun.hpp"
#include "core/gfx/util.hpp"
#include "core/gfx/vertex.hpp"

#include <SDL_surface.h>

namespace GLMeta
{

/* EXT_unpack_subimage */
void subRectImageUpload(GLint srcW, GLint srcX, GLint srcY,
                        GLint dstX, GLint dstY, GLsizei dstW, GLsizei dstH,
                        SDL_Surface *src, GLenum format);
void subRectImageEnd();

/* ARB_vertex_array_object */
struct VAO
{
	/* Set manually, then call vaoInit() */
	const VertexAttribute *attr;
	size_t attrCount;
	GLsizei vertSize;
	VBO::ID vbo;
	IBO::ID ibo;

	/* Don't touch */
	GLuint nativeVAO;
};

template<class VertexType>
inline void vaoFillInVertexData(VAO &vao)
{
	vao.attr      = VertexTraits<VertexType>::attr;
	vao.attrCount = VertexTraits<VertexType>::attrCount;
	vao.vertSize  = sizeof(VertexType);
}

void vaoInit(VAO &vao, bool keepBound = false);
void vaoFini(VAO &vao);
void vaoBind(VAO &vao);
void vaoUnbind(VAO &vao);

/* EXT_framebuffer_blit */
int blitScaleIsSpecial(TEXFBO &target, bool targetPreferHires, const IntRect &targetRect, TEXFBO &source, const IntRect &sourceRect);
int smoothScalingMethod(int scaleIsSpecial);
void blitBegin(TEXFBO &target, bool preferHires = false, int scaleIsOne = 0);
void blitBeginScreen(const Vec2i &size, int scaleIsOne = 0);
void blitSource(TEXFBO &source, int scaleIsOne = 0);
void blitRectangle(const IntRect &src, const Vec2i &dstPos);
void blitRectangle(const IntRect &src, const IntRect &dst,
                   bool smooth = false);
void blitEnd();

}


