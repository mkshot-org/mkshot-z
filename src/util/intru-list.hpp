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

emplate <typename T>
struct IntruListLink
{
	IntruListLink<T> *prev;
	IntruListLink<T> *next;
	T *data;

	IntruListLink(T *data)
	    : prev(0),
	      next(0),
	      data(data)
	{}

	~IntruListLink()
	{
		if (prev && next)
		{
			next->prev = prev;
			prev->next = next;
		}
	}
};

template <typename T>
class IntruList
{
	IntruListLink<T> root;
	int size;

public:
	IntruList()
	    : root(0),
	      size(0)
	{
		root.prev = &root;
		root.next = &root;
	}

	void prepend(IntruListLink<T> &node)
	{
		root.next->prev = &node;
		node.prev = &root;
		node.next = root.next;
		root.next = &node;

		size++;
	}

	void append(IntruListLink<T> &node)
	{
		root.prev->next = &node;
		node.next = &root;
		node.prev = root.prev;
		root.prev = &node;

		size++;
	}

	void insertBefore(IntruListLink<T> &node,
	                  IntruListLink<T> &prev)
	{
		node.next = &prev;
		node.prev = prev.prev;
		prev.prev->next = &node;
		prev.prev = &node;

		size++;
	}

	void remove(IntruListLink<T> &node)
	{
		if (!node.next)
			return;

		node.prev->next = node.next;
		node.next->prev = node.prev;

		node.prev = 0;
		node.next = 0;

		size--;
	}

	void clear()
	{
		remove(root);
		root.prev = &root;
		root.next = &root;

		size = 0;
	}

	T *tail() const
	{
		IntruListLink<T> *node = root.prev;
		if (node == &root)
			return 0;

		return node->data;
	}

	IntruListLink<T> *begin()
	{
		return root.next;
	}

	IntruListLink<T> *end()
	{
		return &root;
	}

	bool isEmpty() const
	{
		return root.next == &root;
	}

	int getSize() const
	{
		return size;
	}
};


