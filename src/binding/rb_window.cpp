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

#include "binding/rb_util.hpp"
#include "binding/rb_disposable.hpp"
#include "binding/rb_viewport.hpp"
#include "core/gfx/window.hpp"

DEF_TYPE(Window);

RB_METHOD(windowInitialize) {
    GFX_LOCK;
    Window *w = viewportElementInitialize<Window>(argc, argv, self);

    setPrivateData(self, w);

    w->initDynAttribs();

    wrapProperty(self, &w->getCursorRect(), "cursor_rect", RectType);

    GFX_UNLOCK;
    return self;
}

RB_METHOD(windowUpdate) {
    RB_UNUSED_PARAM;

    Window *w = getPrivateData<Window>(self);

    GFX_GUARD_EXC(w->update(););

    return Qnil;
}

DEF_GFX_PROP_OBJ_REF(Window, Bitmap, Windowskin, "windowskin")
DEF_GFX_PROP_OBJ_REF(Window, Bitmap, Contents, "contents")
DEF_GFX_PROP_OBJ_VAL(Window, Rect, CursorRect, "cursor_rect")

DEF_GFX_PROP_B(Window, Stretch)
DEF_GFX_PROP_B(Window, Active)
DEF_GFX_PROP_B(Window, Pause)

DEF_GFX_PROP_I(Window, X)
DEF_GFX_PROP_I(Window, Y)
DEF_GFX_PROP_I(Window, Width)
DEF_GFX_PROP_I(Window, Height)
DEF_GFX_PROP_I(Window, OX)
DEF_GFX_PROP_I(Window, OY)
DEF_GFX_PROP_I(Window, Opacity)
DEF_GFX_PROP_I(Window, BackOpacity)
DEF_GFX_PROP_I(Window, ContentsOpacity)

void windowBindingInit() {
    VALUE klass = rb_define_class("Window", rb_cObject);
    rb_define_alloc_func(klass, classAllocate<&WindowType>);

    disposableBindingInit<Window>(klass);
    viewportElementBindingInit<Window>(klass);

    _rb_define_method(klass, "initialize", windowInitialize);
    _rb_define_method(klass, "update", windowUpdate);

    INIT_PROP_BIND(Window, Windowskin, "windowskin");
    INIT_PROP_BIND(Window, Contents, "contents");
    INIT_PROP_BIND(Window, Stretch, "stretch");
    INIT_PROP_BIND(Window, CursorRect, "cursor_rect");
    INIT_PROP_BIND(Window, Active, "active");
    INIT_PROP_BIND(Window, Pause, "pause");
    INIT_PROP_BIND(Window, X, "x");
    INIT_PROP_BIND(Window, Y, "y");
    INIT_PROP_BIND(Window, Width, "width");
    INIT_PROP_BIND(Window, Height, "height");
    INIT_PROP_BIND(Window, OX, "ox");
    INIT_PROP_BIND(Window, OY, "oy");
    INIT_PROP_BIND(Window, Opacity, "opacity");
    INIT_PROP_BIND(Window, BackOpacity, "back_opacity");
    INIT_PROP_BIND(Window, ContentsOpacity, "contents_opacity");
}
