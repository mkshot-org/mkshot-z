/*
** This file is part of mkxp, further modified for mkshot-z.
**
** Copyright (C) 2026 mkshot-z contributors <https://github.com/mkshot-org>
** Copyright (C) 2013 - 2021 Amaryllis Kulla <ancurio@mapleshrine.eu>
**
** mkxp is licensed under GPLv2 or later.
** mkshot-z is licensed under GPLv3 or later.
*/

#pragma once


#include <ruby.h>
#include <ruby/version.h>

#include "util/exception.hpp"

enum RbException {
    RGSS = 0,
    Reset,
    PHYSFS,
    SDL,
    MKSHOT,
    
    ErrnoENOENT,
    
    IOError,
    
    TypeError,
    ArgumentError,
    
    RbExceptionsMax
};

struct RbData {
    VALUE exc[RbExceptionsMax];
    
    /* Input module (RGSS3) */
    VALUE buttoncodeHash;
    
    RbData();
    ~RbData();
};

RbData *getRbData();

struct Exception;

void raiseRbExc(const Exception &exc);

#define DECL_TYPE(Klass) extern rb_data_type_t Klass##Type

/* TODO: can mkxp use RUBY_TYPED_FREE_IMMEDIATELY here? */
#define DEF_TYPE_FLAGS 0

#define DEF_TYPE_CUSTOMNAME_AND_FREE(Klass, Name, Free)                        \
rb_data_type_t Klass##Type = {Name, {0, Free, 0, 0, 0}, 0, 0, DEF_TYPE_FLAGS}

#define DEF_TYPE_CUSTOMFREE(Klass, Free)                                       \
DEF_TYPE_CUSTOMNAME_AND_FREE(Klass, #Klass, Free)

#define DEF_TYPE_CUSTOMNAME(Klass, Name)                                       \
DEF_TYPE_CUSTOMNAME_AND_FREE(Klass, Name, freeInstance<Klass>)

#define DEF_TYPE(Klass) DEF_TYPE_CUSTOMNAME(Klass, #Klass)

template <rb_data_type_t *rbType> static VALUE classAllocate(VALUE klass) {
    return rb_data_typed_object_wrap(klass, 0, rbType);
}

template <class C> static void freeInstance(void *inst) {
    delete static_cast<C *>(inst);
}

void raiseDisposedAccess(VALUE self);

template <class C> inline C *getPrivateData(VALUE self) {
    C *c = static_cast<C *>(RTYPEDDATA_DATA(self));
    if (!c) {
        raiseRbExc(Exception(Exception::MKShotError, "No instance data for variable (missing call to super?)"));
    }
    return c;
}

template <class C>
static inline C *
getPrivateDataCheck(VALUE self, const rb_data_type_t &type)
{
    const char *ownname = rb_obj_classname(self);
    if (!rb_typeddata_is_kind_of(self, &type))
        rb_raise(rb_eTypeError, "Can't convert %s into %s", ownname, type.wrap_struct_name);

    void *obj = RTYPEDDATA_DATA(self);
    return static_cast<C *>(obj);
}

static inline void setPrivateData(VALUE self, void *p) {
    RTYPEDDATA_DATA(self) = p;
}

inline VALUE
wrapObject(void *p, const rb_data_type_t &type, VALUE underKlass = rb_cObject)
{
    VALUE klass = rb_const_get(underKlass, rb_intern(type.wrap_struct_name));
    VALUE obj = rb_obj_alloc(klass);
    
    setPrivateData(obj, p);
    
    return obj;
}

inline VALUE wrapProperty(VALUE self, void *prop, const char *iv, const rb_data_type_t &type,
                          VALUE underKlass = rb_cObject) {
    VALUE propObj = wrapObject(prop, type, underKlass);
    
    rb_iv_set(self, iv, propObj);
    
    return propObj;
}

/* Implemented: oSszfibn| */
int rb_get_args(int argc, VALUE *argv, const char *format, ...);

/* Always terminate 'rb_get_args' with this */
#ifndef NDEBUG
#define RB_ARG_END_VAL ((void *)-1)
#define RB_ARG_END , RB_ARG_END_VAL
#else
#define RB_ARG_END
#endif

typedef VALUE (*RubyMethod)(int argc, VALUE *argv, VALUE self);

static inline void _rb_define_method(VALUE klass, const char *name,
                                     RubyMethod func) {
    rb_define_method(klass, name, RUBY_METHOD_FUNC(func), -1);
}

static inline void rb_define_class_method(VALUE klass, const char *name,
                                          RubyMethod func) {
    rb_define_singleton_method(klass, name, RUBY_METHOD_FUNC(func), -1);
}

static inline void _rb_define_module_function(VALUE module, const char *name,
                                              RubyMethod func) {
    rb_define_module_function(module, name, RUBY_METHOD_FUNC(func), -1);
}

#define GUARD_EXC(exp)                                                         \
{                                                                            \
try {                                                                      \
exp                                                                      \
} catch (const Exception &exc) {                                           \
raiseRbExc(exc);                                                         \
}                                                                          \
}

#define GFX_GUARD_EXC(exp)                                                         \
{\
GFX_LOCK; \
try {\
exp                                                                      \
} catch (const Exception &exc) {\
GFX_UNLOCK; \
raiseRbExc(exc);                                                         \
}\
GFX_UNLOCK;\
}


template <class C>
static inline VALUE objectLoad(int argc, VALUE *argv, VALUE self) {
    const char *data;
    int dataLen;
    rb_get_args(argc, argv, "s", &data, &dataLen RB_ARG_END);
    
    VALUE obj = rb_obj_alloc(self);
    
    C *c = 0;
    
    GUARD_EXC(c = C::deserialize(data, dataLen););
    
    setPrivateData(obj, c);
    
    return obj;
}

static inline VALUE rb_bool_new(bool value) { return value ? Qtrue : Qfalse; }

inline void rb_float_arg(VALUE arg, double *out, int argPos = 0) {
    switch (rb_type(arg)) {
        case RUBY_T_FLOAT:
            *out = RFLOAT_VALUE(arg);
            break;
            
        case RUBY_T_FIXNUM:
            *out = FIX2INT(arg);
            break;
            
        default:
            rb_raise(rb_eTypeError, "Argument %d: Expected float", argPos);
    }
}

inline void rb_int_arg(VALUE arg, int *out, int argPos = 0) {
    switch (rb_type(arg)) {
        case RUBY_T_FLOAT:
            // FIXME check int range?
            *out = NUM2LONG(arg);
            break;
            
        case RUBY_T_FIXNUM:
            *out = FIX2INT(arg);
            break;
            
        default:
            rb_raise(rb_eTypeError, "Argument %d: Expected fixnum", argPos);
    }
}

inline void rb_bool_arg(VALUE arg, bool *out, int argPos = 0) {
    switch (rb_type(arg)) {
        case RUBY_T_TRUE:
            *out = true;
            break;
            
        case RUBY_T_FALSE:
        case RUBY_T_NIL:
            *out = false;
            break;
            
        default:
            rb_raise(rb_eTypeError, "Argument %d: Expected bool", argPos);
    }
}

inline void rb_check_argc(int actual, int expected) {
    if (actual != expected)
        rb_raise(rb_eArgError, "wrong number of arguments (%d for %d)", actual,
                 expected);
}

#define RB_METHOD(name) static VALUE name(int argc, VALUE *argv, VALUE self)

#define RB_UNUSED_PARAM                                                        \
{                                                                            \
(void)argc;                                                                \
(void)argv;                                                                \
(void)self;                                                                \
}

#define MARSH_LOAD_FUN(Typ)                                                    \
RB_METHOD(Typ##Load) { return objectLoad<Typ>(argc, argv, self); }

#define INITCOPY_FUN(Klass)                                                    \
RB_METHOD(Klass##InitializeCopy) {                                           \
VALUE origObj;                                                             \
rb_get_args(argc, argv, "o", &origObj RB_ARG_END);                         \
if (!OBJ_INIT_COPY(self, origObj)) /* When would this fail??*/             \
return self;                                                             \
Klass *orig = getPrivateData<Klass>(origObj);                              \
Klass *k = 0;                                                              \
GUARD_EXC(k = new Klass(*orig);)                                           \
setPrivateData(self, k);                                                   \
return self;                                                               \
}

/* Object property which is copied by reference, with allowed NIL
 * FIXME: Getter assumes prop is disposable,
 * because self.disposed? is not checked in this case.
 * Should make this more clear */

// --------------
// Do not wait for Graphics.update
// --------------
#define DEF_PROP_OBJ_REF(Klass, PropKlass, PropName, prop_iv)                  \
RB_METHOD(Klass##Get##PropName) {                                            \
RB_UNUSED_PARAM;                                                           \
return rb_iv_get(self, prop_iv);                                           \
}                                                                            \
RB_METHOD(Klass##Set##PropName) {                                            \
RB_UNUSED_PARAM;                                                           \
rb_check_argc(argc, 1);                                                    \
Klass *k = getPrivateData<Klass>(self);                                    \
VALUE propObj = *argv;                                                     \
PropKlass *prop;                                                           \
if (NIL_P(propObj))                                                        \
prop = 0;                                                                \
else                                                                       \
prop = getPrivateDataCheck<PropKlass>(propObj, PropKlass##Type);         \
GUARD_EXC(k->set##PropName(prop);)                                         \
rb_iv_set(self, prop_iv, propObj);                                         \
return propObj;                                                            \
}

/* Object property which is copied by value, not reference */
#define DEF_PROP_OBJ_VAL(Klass, PropKlass, PropName, prop_iv)                  \
RB_METHOD(Klass##Get##PropName) {                                            \
RB_UNUSED_PARAM;                                                           \
checkDisposed<Klass>(self);                                                \
return rb_iv_get(self, prop_iv);                                           \
}                                                                            \
RB_METHOD(Klass##Set##PropName) {                                            \
rb_check_argc(argc, 1);                                                    \
Klass *k = getPrivateData<Klass>(self);                                    \
VALUE propObj = *argv;                                                     \
PropKlass *prop;                                                           \
prop = getPrivateDataCheck<PropKlass>(propObj, PropKlass##Type);           \
GUARD_EXC(k->set##PropName(*prop);)                                        \
return propObj;                                                            \
}

#define DEF_PROP(Klass, type, PropName, arg_fun, value_fun)                    \
RB_METHOD(Klass##Get##PropName) {                                            \
RB_UNUSED_PARAM;                                                           \
Klass *k = getPrivateData<Klass>(self);                                    \
type value = 0;                                                            \
GUARD_EXC(value = k->get##PropName();)                                     \
return value_fun(value);                                                   \
}                                                                            \
RB_METHOD(Klass##Set##PropName) {                                            \
rb_check_argc(argc, 1);                                                    \
Klass *k = getPrivateData<Klass>(self);                                    \
type value;                                                                \
rb_##arg_fun##_arg(*argv, &value);                                         \
GUARD_EXC(k->set##PropName(value);)                                        \
return *argv;                                                              \
}

#define DEF_PROP_I(Klass, PropName)                                            \
DEF_PROP(Klass, int, PropName, int, rb_fix_new)

#define DEF_PROP_F(Klass, PropName)                                            \
DEF_PROP(Klass, double, PropName, float, rb_float_new)

#define DEF_PROP_B(Klass, PropName)                                            \
DEF_PROP(Klass, bool, PropName, bool, rb_bool_new)

#define INIT_PROP_BIND(Klass, PropName, prop_name_s)                           \
{                                                                            \
_rb_define_method(klass, prop_name_s, Klass##Get##PropName);               \
_rb_define_method(klass, prop_name_s "=", Klass##Set##PropName);           \
}

// --------------
// Wait for Graphics.update
// --------------
#define DEF_GFX_PROP_OBJ_REF(Klass, PropKlass, PropName, prop_iv)                  \
RB_METHOD(Klass##Get##PropName) {                                            \
RB_UNUSED_PARAM;                                                           \
return rb_iv_get(self, prop_iv);                                           \
}                                                                            \
RB_METHOD(Klass##Set##PropName) {                                            \
RB_UNUSED_PARAM;                                                           \
rb_check_argc(argc, 1);                                                    \
Klass *k = getPrivateData<Klass>(self);                                    \
VALUE propObj = *argv;                                                     \
PropKlass *prop;                                                           \
if (NIL_P(propObj))                                                        \
prop = 0;                                                                \
else                                                                       \
prop = getPrivateDataCheck<PropKlass>(propObj, PropKlass##Type);         \
GFX_GUARD_EXC(k->set##PropName(prop);)                                         \
rb_iv_set(self, prop_iv, propObj);                                         \
return propObj;                                                            \
}

/* Object property which is copied by value, not reference */
#define DEF_GFX_PROP_OBJ_VAL(Klass, PropKlass, PropName, prop_iv)                  \
RB_METHOD(Klass##Get##PropName) {                                            \
RB_UNUSED_PARAM;                                                           \
checkDisposed<Klass>(self);                                                \
return rb_iv_get(self, prop_iv);                                           \
}                                                                            \
RB_METHOD(Klass##Set##PropName) {                                            \
rb_check_argc(argc, 1);                                                    \
Klass *k = getPrivateData<Klass>(self);                                    \
VALUE propObj = *argv;                                                     \
PropKlass *prop;                                                           \
prop = getPrivateDataCheck<PropKlass>(propObj, PropKlass##Type);           \
GFX_GUARD_EXC(k->set##PropName(*prop);)                                        \
return propObj;                                                            \
}

#define DEF_GFX_PROP(Klass, type, PropName, arg_fun, value_fun)                    \
RB_METHOD(Klass##Get##PropName) {                                            \
RB_UNUSED_PARAM;                                                           \
Klass *k = getPrivateData<Klass>(self);                                    \
type value = 0;                                                            \
GUARD_EXC(value = k->get##PropName();)                                     \
return value_fun(value);                                                   \
}                                                                            \
RB_METHOD(Klass##Set##PropName) {                                            \
rb_check_argc(argc, 1);                                                    \
Klass *k = getPrivateData<Klass>(self);                                    \
type value;                                                                \
rb_##arg_fun##_arg(*argv, &value);                                         \
GFX_GUARD_EXC(k->set##PropName(value);)                                        \
return *argv;                                                              \
}

#define DEF_GFX_PROP_I(Klass, PropName)                                            \
DEF_GFX_PROP(Klass, int, PropName, int, rb_fix_new)

#define DEF_GFX_PROP_F(Klass, PropName)                                            \
DEF_GFX_PROP(Klass, double, PropName, float, rb_float_new)

#define DEF_GFX_PROP_B(Klass, PropName)                                            \
DEF_GFX_PROP(Klass, bool, PropName, bool, rb_bool_new)


