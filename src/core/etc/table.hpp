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


#include "util/serializable.hpp"

#include <stdint.h>
#include "sigslot.hpp"
#include <vector>

class Table : public Serializable
{
public:
	Table(int x, int y = 1, int z = 1);
	/* Clone constructor */
	Table(const Table &other);
	virtual ~Table() {}

	int xSize() const { return xs; }
	int ySize() const { return ys; }
	int zSize() const { return zs; }

	int16_t get(int x, int y = 0, int z = 0) const;
	void set(int16_t value, int x, int y = 0, int z = 0);

	void resize(int x, int y, int z);
	void resize(int x, int y);
	void resize(int x);

	int serialSize() const;
	void serialize(char *buffer) const;
	static Table *deserialize(const char *data, int len);

	/* <internal */
	inline int16_t &at(int x, int y = 0, int z = 0)
	{
		return data[xs*ys*z + xs*y + x];
	}

	inline const int16_t &at(int x, int y = 0, int z = 0) const
	{
		return data[xs*ys*z + xs*y + x];
	}

    sigslot::signal<> modified;

private:
	int xs, ys, zs;
	std::vector<int16_t> data;
};


