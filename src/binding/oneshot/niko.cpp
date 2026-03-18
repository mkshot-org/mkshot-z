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

#include "binding/util.hpp"
#include "core/shared-state.hpp"
#include "core/sys/sys.hpp"
#include "core/oneshot/oneshot.hpp"
#include "core/oneshot/journal.hpp"

RB_METHOD(nikoPrepare)
{
	RB_UNUSED_PARAM;

#ifndef SDL_PLATFORM_WIN32
	// Needed on non-Windows to prepare (start) Journal program
	shState->oneshot().journal->nikoPrepare();
#endif

	return Qnil;
}

RB_METHOD(nikoStart)
{
	RB_UNUSED_PARAM;

	shState->oneshot().journal->nikoStart();

	return Qnil;
}

void nikoBindingInit()
{
	VALUE module = rb_define_module("Niko");

	_rb_define_module_function(module, "get_ready", nikoPrepare);
	_rb_define_module_function(module, "do_your_thing", nikoStart);
}
