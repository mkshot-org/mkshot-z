/*
** This file is part of mkxp, further modified for mkshot-z.
**
** Copyright (C) 2026 mkshot-z contributors <https://github.com/mkshot-org>
** Copyright (C) 2013 - 2021 Amaryllis Kulla <ancurio@mapleshrine.eu>
**
** mkxp is licensed under GPLv2 or later.
** mkshot-z is licensed under GPLv3 or later.
*/

#include "binding/util.hpp"
#include "binding/disposable.hpp"
#include "binding/viewport.hpp"
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
