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

#include "util/intru-list.hpp"
#include "util/exception.hpp"
#include "core/shared-state.hpp"
#include "core/gfx/gfx.hpp"

#include <assert.h>
#include "sigslot.hpp"

class Disposable
{
public:
	Disposable()
	    : disposed(false),
	      link(this)
	{
		shState->graphics().addDisposable(this);
	}

	virtual ~Disposable()
	{
		shState->graphics().remDisposable(this);
	}

	void dispose()
	{
		if (disposed)
			return;

		releaseResources();
		disposed = true;
		wasDisposed();
	}

	bool isDisposed() const
	{
		return disposed;
	}

    sigslot::signal<> wasDisposed;

protected:
	void guardDisposed() const
	{
		if (isDisposed())
			throw Exception(Exception::RGSSError,
		                    "disposed %s", klassName());
	}

private:
	virtual void releaseResources() = 0;
	virtual const char *klassName() const = 0;

	friend class Graphics;

	bool disposed;
	IntruListLink<Disposable> link;
};

template<class C>
inline bool
nullOrDisposed(const C *d)
{
	if (!d)
		return true;

	if (d->isDisposed())
		return true;

	return false;
}


