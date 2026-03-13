/*
** This file is part of mkxp, further modified for mkshot-z.
**
** Copyright (C) 2026 mkshot-z contributors <https://github.com/mkshot-org>
** Copyright (C) 2014 - 2021 Amaryllis Kulla <ancurio@mapleshrine.eu>
**
** mkxp is licensed under GPLv2 or later.
** mkshot-z is licensed under GPLv3 or later.
*/

#include "core/gfx/bitmap.hpp"
#include "core/shared-state.hpp"
#include "core/etc/table.hpp"
#include "core/gfx/tilemap-vx.hpp"
#include "core/gfx/viewport.hpp"

#include "binding/types.hpp"
#include "binding/util.hpp"
#include "binding/disposable.hpp"

DEF_TYPE_CUSTOMNAME(TilemapVX, "Tilemap");
DEF_TYPE_CUSTOMFREE(BitmapArray, RUBY_TYPED_NEVER_FREE);

RB_METHOD(tilemapVXInitialize) {
    TilemapVX *t;
    
    GFX_LOCK;
    /* Get parameters */
    VALUE viewportObj = Qnil;
    Viewport *viewport = 0;
    
    rb_get_args(argc, argv, "|o", &viewportObj RB_ARG_END);
    
    if (!NIL_P(viewportObj))
        viewport = getPrivateDataCheck<Viewport>(viewportObj, ViewportType);
    
    /* Construct object */
    t = new TilemapVX(viewport);
    
    setPrivateData(self, t);
    
    rb_iv_set(self, "viewport", viewportObj);
    
    wrapProperty(self, &t->getBitmapArray(), "bitmap_array", BitmapArrayType,
                 rb_const_get(rb_cObject, rb_intern("Tilemap")));
    
    VALUE autotilesObj = rb_iv_get(self, "bitmap_array");
    
    VALUE ary = rb_ary_new2(9);
    for (int i = 0; i < 9; ++i)
        rb_ary_push(ary, Qnil);
    
    rb_iv_set(autotilesObj, "array", ary);
    
    /* Circular reference so both objects are always
     * alive at the same time */
    rb_iv_set(autotilesObj, "tilemap", self);
    
    GFX_UNLOCK;
    return self;
}

RB_METHOD(tilemapVXGetBitmapArray) {
    RB_UNUSED_PARAM;
    
    checkDisposed<TilemapVX>(self);
    
    return rb_iv_get(self, "bitmap_array");
}

RB_METHOD(tilemapVXUpdate) {
    RB_UNUSED_PARAM;
    
    TilemapVX *t = getPrivateData<TilemapVX>(self);
    
    t->update();
    
    return Qnil;
}

DEF_GFX_PROP_OBJ_REF(TilemapVX, Viewport, Viewport, "viewport")
DEF_GFX_PROP_OBJ_REF(TilemapVX, Table, MapData, "map_data")
DEF_GFX_PROP_OBJ_REF(TilemapVX, Table, FlashData, "flash_data")
DEF_GFX_PROP_OBJ_REF(TilemapVX, Table, Flags, "flags")

DEF_GFX_PROP_B(TilemapVX, Visible)

DEF_GFX_PROP_I(TilemapVX, OX)
DEF_GFX_PROP_I(TilemapVX, OY)

RB_METHOD(tilemapVXBitmapsSet) {
    TilemapVX::BitmapArray *a = getPrivateData<TilemapVX::BitmapArray>(self);
    
    int i;
    VALUE bitmapObj;
    
    rb_get_args(argc, argv, "io", &i, &bitmapObj RB_ARG_END);
    
    Bitmap *bitmap = getPrivateDataCheck<Bitmap>(bitmapObj, BitmapType);
    
    GFX_LOCK;
    a->set(i, bitmap);
    
    VALUE ary = rb_iv_get(self, "array");
    rb_ary_store(ary, i, bitmapObj);
    GFX_UNLOCK;
    return self;
}

RB_METHOD(tilemapVXBitmapsGet) {
    int i;
    rb_get_args(argc, argv, "i", &i RB_ARG_END);
    
    if (i < 0 || i > 8)
        return Qnil;
    
    VALUE ary = rb_iv_get(self, "array");
    
    return rb_ary_entry(ary, i);
}

void tilemapVXBindingInit() {
    VALUE klass = rb_define_class("Tilemap", rb_cObject);
    rb_define_alloc_func(klass, classAllocate<&TilemapVXType>);
    
    disposableBindingInit<TilemapVX>(klass);
    
    _rb_define_method(klass, "initialize", tilemapVXInitialize);
    _rb_define_method(klass, "bitmaps", tilemapVXGetBitmapArray);
    _rb_define_method(klass, "update", tilemapVXUpdate);
    
    INIT_PROP_BIND(TilemapVX, Viewport, "viewport");
    INIT_PROP_BIND(TilemapVX, MapData, "map_data");
    INIT_PROP_BIND(TilemapVX, FlashData, "flash_data");
    INIT_PROP_BIND(TilemapVX, Visible, "visible");
    INIT_PROP_BIND(TilemapVX, OX, "ox");
    INIT_PROP_BIND(TilemapVX, OY, "oy");
    
    if (rgssVer == 3) {
        INIT_PROP_BIND(TilemapVX, Flags, "flags");
    } else {
        INIT_PROP_BIND(TilemapVX, Flags, "passages");
    }
    
    klass = rb_define_class_under(klass, "BitmapArray", rb_cObject);
    rb_define_alloc_func(klass, classAllocate<&BitmapArrayType>);
    
    _rb_define_method(klass, "[]=", tilemapVXBitmapsSet);
    _rb_define_method(klass, "[]", tilemapVXBitmapsGet);
}
