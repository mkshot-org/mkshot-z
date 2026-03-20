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

#include <cstddef>

#include "core/oneshot/gnome-fun.hpp"

#ifndef __XFCONF_CHANNEL_H__
typedef struct _XfconfChannel XfconfChannel;
#endif

/* Xfconf prototypes */
typedef gboolean (*XFCONFINITPROC)(GError **error);
typedef void (*XFCONFSHUTDOWNPROC)(void);
typedef XfconfChannel *(*XFCONFCHANNELGETPROC)(const gchar *channel_name);
typedef gchar *(*XFCONFCHANNELGETSTRINGPROC)(XfconfChannel *channel, const gchar *property, const gchar *default_value);
typedef gboolean (*XFCONFCHANNELSETSTRINGPROC)(XfconfChannel *channel, const gchar *property, const gchar *value);
typedef gint32 (*XFCONFCHANNELGETINTPROC)(XfconfChannel *channel, const gchar *property, gint32 default_value);
typedef gboolean (*XFCONFCHANNELSETINTPROC)(XfconfChannel *channel, const gchar *property, gint32 value);
typedef gboolean (*XFCONFCHANNELGETARRAYPROC)(XfconfChannel *channel, const gchar *property, GType first_value_type, ...);
typedef gboolean (*XFCONFCHANNELSETARRAYPROC)(XfconfChannel *channel, const gchar *property, GType first_value_type, ...);
typedef void (*XFCONFCHANNELRESETPROPERTYPROC)(XfconfChannel *channel, const gchar *property_base, gboolean recursive);

#define XFCONF_FUNCS \
	XFCONF_FUNC(init, XFCONFINITPROC) \
	XFCONF_FUNC(shutdown, XFCONFSHUTDOWNPROC) \
	XFCONF_FUNC(channel_get, XFCONFCHANNELGETPROC) \
	XFCONF_FUNC(channel_get_string, XFCONFCHANNELGETSTRINGPROC) \
	XFCONF_FUNC(channel_set_string, XFCONFCHANNELSETSTRINGPROC) \
	XFCONF_FUNC(channel_get_int, XFCONFCHANNELGETINTPROC) \
	XFCONF_FUNC(channel_set_int, XFCONFCHANNELSETINTPROC) \
	XFCONF_FUNC(channel_get_array, XFCONFCHANNELGETARRAYPROC) \
	XFCONF_FUNC(channel_set_array, XFCONFCHANNELSETARRAYPROC) \
	XFCONF_FUNC(channel_reset_property, XFCONFCHANNELRESETPROPERTYPROC)

struct XfconfFunctions
{
#define XFCONF_FUNC(name, type) type name;
	XFCONF_FUNCS
#undef XFCONF_FUNC
};

#define HAVE_XFCONF dynXfconf.init

extern XfconfFunctions dynXfconf;

void initXfconfFunctions();
