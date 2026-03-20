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

#include "core/gfx/gl-util.hpp"

#include <vector>
#include <limits>
#include <assert.h>
#include <stdint.h>

typedef uint16_t index_t;
#define INDEX_T_MAX std::numeric_limits<index_t>::max()
#define _GL_INDEX_TYPE GL_UNSIGNED_SHORT

struct GlobalIBO
{
	IBO::ID ibo;
	std::vector<index_t> buffer;

	GlobalIBO()
	{
		ibo = IBO::gen();
	}

	~GlobalIBO()
	{
		IBO::del(ibo);
	}

	void ensureSize(size_t quadCount)
	{
		assert(quadCount*6 < INDEX_T_MAX);

		if (buffer.size() >= quadCount*6)
			return;

		size_t startInd = buffer.size() / 6;
		buffer.reserve(quadCount*6);

		for (size_t i = startInd; i < quadCount; ++i)
		{
			static const index_t indTemp[] = { 0, 1, 2, 2, 3, 0 };

			for (size_t j = 0; j < 6; ++j)
				buffer.push_back(i * 4 + indTemp[j]);
		}

		IBO::bind(ibo);
		IBO::uploadData(buffer.size() * sizeof(index_t), dataPtr(buffer));
		IBO::unbind();
	}
};


