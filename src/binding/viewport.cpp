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

#include "binding/types.hpp"
#include "binding/util.hpp"
#include "binding/disposable.hpp"
#include "binding/flashable.hpp"
#include "binding/scene.hpp"
#include "core/shared-state.hpp"
#include "core/gfx/viewport.hpp"

DEF_TYPE(Viewport);

RB_METHOD(viewportInitialize) {
    Viewport *v;
    
    if (argc == 0 && rgssVer >= 3) {
        GFX_LOCK;
        v = new Viewport();
    } else if (argc == 1) {
        /* The rect arg is only used to init the viewport,
         * and does NOT replace its 'rect' property */
        VALUE rectObj;
        Rect *rect;
        
        rb_get_args(argc, argv, "o", &rectObj RB_ARG_END);
        
        rect = getPrivateDataCheck<Rect>(rectObj, RectType);
        
        GFX_LOCK;
        v = new Viewport(rect);
    } else {
        int x, y, width, height;
        
        rb_get_args(argc, argv, "iiii", &x, &y, &width, &height RB_ARG_END);
        GFX_LOCK;
        v = new Viewport(x, y, width, height);
    }
    
    setPrivateData(self, v);
    
    /* Wrap property objects */
    v->initDynAttribs();
    
    wrapProperty(self, &v->getRect(), "rect", RectType);
    wrapProperty(self, &v->getColor(), "color", ColorType);
    wrapProperty(self, &v->getTone(), "tone", ToneType);
    
    GFX_UNLOCK;
    return self;
}

DEF_GFX_PROP_OBJ_VAL(Viewport, Rect, Rect, "rect")
DEF_GFX_PROP_OBJ_VAL(Viewport, Color, Color, "color")
DEF_GFX_PROP_OBJ_VAL(Viewport, Tone, Tone, "tone")

DEF_GFX_PROP_I(Viewport, OX)
DEF_GFX_PROP_I(Viewport, OY)

void viewportBindingInit() {
    VALUE klass = rb_define_class("Viewport", rb_cObject);
    rb_define_alloc_func(klass, classAllocate<&ViewportType>);
    
    disposableBindingInit<Viewport>(klass);
    flashableBindingInit<Viewport>(klass);
    sceneElementBindingInit<Viewport>(klass);
    
    _rb_define_method(klass, "initialize", viewportInitialize);
    
    INIT_PROP_BIND(Viewport, Rect, "rect");
    INIT_PROP_BIND(Viewport, OX, "ox");
    INIT_PROP_BIND(Viewport, OY, "oy");
    INIT_PROP_BIND(Viewport, Color, "color");
    INIT_PROP_BIND(Viewport, Tone, "tone");
}
