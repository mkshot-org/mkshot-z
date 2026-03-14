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

#include "core/etc/table.hpp"

#include <string.h>
#include <algorithm>

#include "util/serial.hpp"
#include "util/exception.hpp"
#include "util/util.hpp"

/* Init normally */
Table::Table(int x, int y /*= 1*/, int z /*= 1*/)
    : xs(x), ys(y), zs(z),
      data(x*y*z)
{}

Table::Table(const Table &other)
    : xs(other.xs), ys(other.ys), zs(other.zs),
      data(other.data)
{}

int16_t Table::get(int x, int y, int z) const
{
	return data[xs*ys*z + xs*y + x];
}

void Table::set(int16_t value, int x, int y, int z)
{
	if (x < 0 || x >= xs
	||  y < 0 || y >= ys
	||  z < 0 || z >= zs)
	{
		return;
	}

	data[xs*ys*z + xs*y + x] = value;

	modified();
}

void Table::resize(int x, int y, int z)
{
	if (x == xs && y == ys && z == zs)
		return;

	std::vector<int16_t> newData(x*y*z);

	for (int k = 0; k < std::min(z, zs); ++k)
		for (int j = 0; j < std::min(y, ys); ++j)
			for (int i = 0; i < std::min(x, xs); ++i)
				newData[x*y*k + x*j + i] = at(i, j, k);

	data.swap(newData);

	xs = x;
	ys = y;
	zs = z;

	return;
}

void Table::resize(int x, int y)
{
	resize(x, y, zs);
}

void Table::resize(int x)
{
	resize(x, ys, zs);
}

/* Serializable */
int Table::serialSize() const
{
	/* header + data */
	return 20 + (xs * ys * zs) * 2;
}

void Table::serialize(char *buffer) const
{
	/* Table dimensions: we don't care
	 * about them but RMXP needs them */
	int dim = 1;
	int size = xs * ys * zs;

	if (ys > 1)
		dim = 2;

	if (zs > 1)
		dim = 3;

	writeInt32(&buffer, dim);
	writeInt32(&buffer, xs);
	writeInt32(&buffer, ys);
	writeInt32(&buffer, zs);
	writeInt32(&buffer, size);

	memcpy(buffer, dataPtr(data), sizeof(int16_t)*size);
}


Table *Table::deserialize(const char *data, int len)
{
	if (len < 20)
		throw Exception(Exception::RGSSError, "Marshal: Table: bad file format");

	readInt32(&data);
	int x = readInt32(&data);
	int y = readInt32(&data);
	int z = readInt32(&data);
	int size = readInt32(&data);

	if (size != x*y*z)
		throw Exception(Exception::RGSSError, "Marshal: Table: bad file format");

	if (len != 20 + x*y*z*2)
		throw Exception(Exception::RGSSError, "Marshal: Table: bad file format");

	Table *t = new Table(x, y, z);
	memcpy(dataPtr(t->data), data, sizeof(int16_t)*size);

	return t;
}
