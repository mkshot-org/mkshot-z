/*
** This file is part of mkxp, further modified for mkshot-z.
**
** Copyright (C) 2026 mkshot-z contributors <https://github.com/mkshot-org>
** Copyright (C) 2013 - 2021 Amaryllis Kulla <ancurio@mapleshrine.eu>
**
** mkxp is licensed under GPLv2 or later.
** mkshot-z is licensed under GPLv3 or later.
*/

#include "core/config.hpp"

#include "binding/util.hpp"

#include "core/fs/fs.hpp"
#include "core/shared-state.hpp"
#include "util/util.hpp"

#include <ruby/encoding.h>
#include <ruby/intern.h>
#include <ruby/thread.h>

static void fileIntFreeInstance(void *inst) {
    SDL_RWops *ops = static_cast<SDL_RWops *>(inst);
    
    SDL_RWclose(ops);
    SDL_FreeRW(ops);
}

DEF_TYPE_CUSTOMFREE(FileInt, fileIntFreeInstance);

static VALUE fileIntForPath(const char *path, bool rubyExc) {
    SDL_RWops *ops = SDL_AllocRW();
    
    try {
        shState->fileSystem().openReadRaw(*ops, path);
    } catch (const Exception &e) {
        SDL_FreeRW(ops);
        
        if (rubyExc)
            raiseRbExc(e);
        else
            throw e;
    }
    
    VALUE klass = rb_const_get(rb_cObject, rb_intern("FileInt"));
    
    VALUE obj = rb_obj_alloc(klass);
    
    setPrivateData(obj, ops);
    
    return obj;
}

typedef struct {
    SDL_RWops *ops;
    void *dst;
    int length;
} fileIntReadCbArgs;

void call_RWread_cb(fileIntReadCbArgs *args) {
    SDL_RWread(args->ops, args->dst, 1, args->length);
}

RB_METHOD(fileIntRead) {
    
    int length = -1;
    rb_get_args(argc, argv, "|i", &length RB_ARG_END);
    
    SDL_RWops *ops = getPrivateData<SDL_RWops>(self);
    
    if (length == -1) {
        Sint64 cur = SDL_RWtell(ops);
        Sint64 end = SDL_RWseek(ops, 0, SEEK_END);
        
        // Sometimes SDL_RWseek will fail for no reason
        // with encrypted archives, so let's just ask
        // for the size up front
        if (end < 0)
            end = ops->size(ops);
        
        length = end - cur;
        SDL_RWseek(ops, cur, SEEK_SET);
    }
    
    if (length == 0)
        return Qnil;
    
    VALUE data = rb_str_new(0, length);
    
    
    
    fileIntReadCbArgs cbargs {ops, RSTRING_PTR(data), length};
    rb_thread_call_without_gvl([](void* args) -> void* {
        call_RWread_cb((fileIntReadCbArgs*)args);
        return 0;
    }, (void*)&cbargs, 0, 0);
    
    return data;
}

RB_METHOD(fileIntClose) {
    RB_UNUSED_PARAM;
    
    SDL_RWops *ops = getPrivateData<SDL_RWops>(self);
    SDL_RWclose(ops);
    
    return Qnil;
}

RB_METHOD(fileIntGetByte) {
    RB_UNUSED_PARAM;
    
    SDL_RWops *ops = getPrivateData<SDL_RWops>(self);
    
    unsigned char byte;
    size_t result = SDL_RWread(ops, &byte, 1, 1);
    
    return (result == 1) ? rb_fix_new(byte) : Qnil;
}

RB_METHOD(fileIntBinmode) {
    RB_UNUSED_PARAM;
    
    return Qnil;
}


VALUE
kernelLoadDataInt(const char *filename, bool rubyExc, bool raw) {
    //rb_gc_start();
    
    VALUE port = fileIntForPath(filename, rubyExc);
    VALUE result;
    if (!raw) {
        VALUE marsh = rb_const_get(rb_cObject, rb_intern("Marshal"));
        
        // FIXME need to catch exceptions here with begin rescue
        VALUE data = fileIntRead(0, 0, port);
        result = rb_funcall2(marsh, rb_intern("load"), 1, &data);
    } else {
        result = fileIntRead(0, 0, port);
    }
    
    rb_funcall2(port, rb_intern("close"), 0, NULL);
    
    return result;
}

RB_METHOD(kernelLoadData) {
    RB_UNUSED_PARAM;
    
    VALUE filename;
    VALUE raw;
    rb_scan_args(argc, argv, "11", &filename, &raw);
    SafeStringValue(filename);
    
    bool rawv;
    rb_bool_arg(raw, &rawv);
    return kernelLoadDataInt(RSTRING_PTR(filename), true, rawv);
}

RB_METHOD(kernelSaveData) {
    RB_UNUSED_PARAM;
    
    VALUE obj;
    VALUE filename;
    
    rb_get_args(argc, argv, "oS", &obj, &filename RB_ARG_END);
    
    VALUE file = rb_file_open_str(filename, "wb");
    
    VALUE marsh = rb_const_get(rb_cObject, rb_intern("Marshal"));
    
    VALUE v[] = {obj, file};
    rb_funcall2(marsh, rb_intern("dump"), ARRAY_SIZE(v), v);
    
    rb_io_close(file);
    
    return Qnil;
}
static VALUE stringForceUTF8(RB_BLOCK_CALL_FUNC_ARGLIST(arg, callback_arg))
{
    if (RB_TYPE_P(arg, RUBY_T_STRING) && ENCODING_IS_ASCII8BIT(arg))
        rb_enc_associate_index(arg, rb_utf8_encindex());
    
    return arg;
}

RB_METHOD(_marshalLoad) {
    RB_UNUSED_PARAM;
    VALUE port;
    rb_get_args(argc, argv, "o", &port RB_ARG_END);
    
    VALUE utf8Proc;
    utf8Proc = rb_proc_new(stringForceUTF8, Qnil);
    
    VALUE marsh = rb_const_get(rb_cObject, rb_intern("Marshal"));
    
    VALUE v[] = {port, utf8Proc};
    return rb_funcall2(marsh, rb_intern("_mkshot_load_alias"), ARRAY_SIZE(v), v);
}

void fileIntBindingInit() {
    VALUE klass = rb_define_class("FileInt", rb_cIO);
    rb_define_alloc_func(klass, classAllocate<&FileIntType>);
    
    _rb_define_method(klass, "read", fileIntRead);
    _rb_define_method(klass, "getbyte", fileIntGetByte);
    _rb_define_method(klass, "binmode", fileIntBinmode);
    _rb_define_method(klass, "close", fileIntClose);
    
    _rb_define_module_function(rb_mKernel, "load_data", kernelLoadData);
    _rb_define_module_function(rb_mKernel, "save_data", kernelSaveData);
    
    /* We overload the built-in 'Marshal::load()' function to silently
     * insert our utf8proc that ensures all read strings will be
     * UTF-8 encoded */
    VALUE marsh = rb_const_get(rb_cObject, rb_intern("Marshal"));
    rb_define_alias(rb_singleton_class(marsh), "_mkshot_load_alias", "load");
    _rb_define_module_function(marsh, "load", _marshalLoad);
}
