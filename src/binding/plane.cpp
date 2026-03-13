/*
** This file is part of mkxp, further modified for mkshot-z.
**
** Copyright (C) 2026 mkshot-z contributors <https://github.com/mkshot-org>
** Copyright (C) 2013 - 2021 Amaryllis Kulla <ancurio@mapleshrine.eu>
**
** mkxp is licensed under GPLv2 or later.
** mkshot-z is licensed under GPLv3 or later.
*/

#include "binding/types.hpp"
#include "binding/util.hpp"
#include "binding/disposable.hpp"
#include "binding/plane.hpp"
#include "binding/viewport.hpp"

DEF_TYPE(Plane);

RB_METHOD(planeInitialize)
{
	Plane *p = viewportElementInitialize<Plane>(argc, argv, self);

	setPrivateData(self, p);

	GFX_LOCK;
	p->initDynAttribs();
	wrapProperty(self, &p->getColor(), "color", ColorType);
	wrapProperty(self, &p->getTone(), "tone", ToneType);
	wrapProperty(self, &p->getSrcRect(), "src_rect", RectType);
	GFX_UNLOCK;

	return self;
}

DEF_GFX_PROP_OBJ_REF(Plane, Bitmap, Bitmap, "bitmap")
DEF_GFX_PROP_OBJ_VAL(Plane, Color, Color, "color")
DEF_GFX_PROP_OBJ_VAL(Plane, Tone, Tone, "tone")
DEF_GFX_PROP_OBJ_VAL(Plane, Rect, SrcRect, "src_rect")

DEF_GFX_PROP_I(Plane, OX)
DEF_GFX_PROP_I(Plane, OY)
DEF_GFX_PROP_I(Plane, Opacity)
DEF_GFX_PROP_I(Plane, BlendType)

DEF_GFX_PROP_F(Plane, ZoomX)
DEF_GFX_PROP_F(Plane, ZoomY)

void planeBindingInit() {
  VALUE klass = rb_define_class("Plane", rb_cObject);
  rb_define_alloc_func(klass, classAllocate<&PlaneType>);

  disposableBindingInit<Plane>(klass);
  viewportElementBindingInit<Plane>(klass);

  _rb_define_method(klass, "initialize", planeInitialize);

  INIT_PROP_BIND(Plane, Bitmap, "bitmap");
  INIT_PROP_BIND(Plane, OX, "ox");
  INIT_PROP_BIND(Plane, OY, "oy");
  INIT_PROP_BIND(Plane, ZoomX, "zoom_x");
  INIT_PROP_BIND(Plane, ZoomY, "zoom_y");
  INIT_PROP_BIND(Plane, Opacity, "opacity");
  INIT_PROP_BIND(Plane, BlendType, "blend_type");
  INIT_PROP_BIND(Plane, Color, "color");
  INIT_PROP_BIND(Plane, Tone, "tone");
  INIT_PROP_BIND(Plane, SrcRect, "src_rect");
}
