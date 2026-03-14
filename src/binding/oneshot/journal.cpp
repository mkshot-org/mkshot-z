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
#include "core/oneshot/i18n.hpp"
#include "core/oneshot/oneshot.hpp"
#include "core/oneshot/journal.hpp"

RB_METHOD(journalActive)
{
	RB_UNUSED_PARAM;

	return shState->oneshot().journal->isActive() ? Qtrue : Qfalse;
}

RB_METHOD(journalSet)
{
	RB_UNUSED_PARAM;

	const char *name;
	rb_get_args(argc, argv, "z", &name RB_ARG_END);

	shState->oneshot().journal->set(name);

	return Qnil;
}

RB_METHOD(journalSetLang)
{
	RB_UNUSED_PARAM;

	const char *lang;
	rb_get_args(argc, argv, "z", &lang RB_ARG_END);

	shState->oneshot().journal->setLang(lang);

	OneshotImpl::i18n::loadLocale(lang);

	return Qnil;
}

void journalBindingInit()
{
	VALUE module = rb_define_module("Journal");

	_rb_define_module_function(module, "active?", journalActive);
	_rb_define_module_function(module, "set", journalSet);
	_rb_define_module_function(module, "setLang", journalSetLang);
}
