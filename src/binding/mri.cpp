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

#include "core/audio/audio.hpp"
#include "core/fs/fs.hpp"
#include "core/gfx/gfx.hpp"
#include "core/gfx/font.hpp"
#include "core/sys/sys.hpp"

#include "util/util.hpp"
#include "util/sdl.hpp"
#include "util/dbg-writer.hpp"
#include "util/boost-hash.hpp"
#include "util/exception.hpp"

#include "core/config.hpp"

#include "binding/rb_util.hpp"
#include "binding/binding.hpp"

#include "core/shared-state.hpp"
#include "core/event-thread.hpp"

#include <vector>

extern "C" {
#include <ruby.h>
#include <ruby/encoding.h>
}

#ifdef __WIN32__
#include "binding/mri-win32.hpp"
#endif

#include <assert.h>
#include <string>

#include <SDL3/SDL_cpuinfo.h>
#include <SDL3/SDL_filesystem.h>
#include <SDL3/SDL_loadso.h>
#include <SDL3/SDL_power.h>

extern const char module_rgss1[];
extern const char module_rgss2[];
extern const char module_rgss3[];

static void mriBindingExecute();
static void mriBindingTerminate();
static void mriBindingReset();

ScriptBinding scriptBindingImpl = {mriBindingExecute, mriBindingTerminate,
    mriBindingReset};

ScriptBinding *scriptBinding = &scriptBindingImpl;

void tableBindingInit();
void etcBindingInit();
void fontBindingInit();
void bitmapBindingInit();
void spriteBindingInit();
void viewportBindingInit();
void planeBindingInit();
void windowBindingInit();
void tilemapBindingInit();
void windowVXBindingInit();
void tilemapVXBindingInit();

void inputBindingInit();
void audioBindingInit();
void graphicsBindingInit();

void fileIntBindingInit();

/*
#ifdef MKSHOT_STEAM
void CUSLBindingInit();
#endif
*/

void httpBindingInit();

// OneShot bindings
void oneshotBindingInit();
void journalBindingInit();
void nikoBindingInit();
void wallpaperBindingInit();

void steamBindingInit();

RB_METHOD(mkshotDelta);
RB_METHOD(mriPrint);
RB_METHOD(mriP);
RB_METHOD(mkshotDataDirectory);
RB_METHOD(mkshotSetTitle);
RB_METHOD(mkshotGetTitle);
RB_METHOD(mkshotDesensitize);
RB_METHOD(mkshotPuts);

RB_METHOD(mkshotPlatform);
RB_METHOD(mkshotIsMacHost);
RB_METHOD(mkshotIsWindowsHost);
RB_METHOD(mkshotIsLinuxHost);
RB_METHOD(mkshotIsUsingRosetta);
RB_METHOD(mkshotIsUsingWine);
RB_METHOD(mkshotIsReallyMacHost);
RB_METHOD(mkshotIsReallyLinuxHost);
RB_METHOD(mkshotIsReallyWindowsHost);

RB_METHOD(mkshotUserLanguage);
RB_METHOD(mkshotUserName);
RB_METHOD(mkshotUserFullName);

RB_METHOD(mkshotGameTitle);
RB_METHOD(mkshotPowerState);
RB_METHOD(mkshotSettingsMenu);
RB_METHOD(mkshotCpuCount);
RB_METHOD(mkshotSystemMemory);
RB_METHOD(mkshotReloadPathCache);
RB_METHOD(mkshotAddPath);
RB_METHOD(mkshotRemovePath);
RB_METHOD(mkshotFileExists);
RB_METHOD(mkshotLaunch);

RB_METHOD(mkshotGetJSONSetting);
RB_METHOD(mkshotSetJSONSetting);
RB_METHOD(mkshotGetAllJSONSettings);

RB_METHOD(mkshotSetDefaultFontFamily);

RB_METHOD(mriRgssMain);
RB_METHOD(mriRgssStop);
RB_METHOD(_kernelCaller);

VALUE json2rb(json5pp::value const &v);
json5pp::value rb2json(VALUE v);

static void mriBindingInit() {
    tableBindingInit();
    etcBindingInit();
    fontBindingInit();
    bitmapBindingInit();
    spriteBindingInit();
    viewportBindingInit();
    planeBindingInit();

    if (rgssVer == 1) {
        windowBindingInit();
        tilemapBindingInit();
    } else {
        windowVXBindingInit();
        tilemapVXBindingInit();
    }

    inputBindingInit();
    audioBindingInit();
    graphicsBindingInit();

    fileIntBindingInit();

    /*
#ifdef MKSHOT_STEAM
    CUSLBindingInit();
#endif
    */

    httpBindingInit();

    // OneShot bindings
    oneshotBindingInit();
    journalBindingInit();
    nikoBindingInit();
    wallpaperBindingInit();

    steamBindingInit();

    if (rgssVer >= 3) {
        _rb_define_module_function(rb_mKernel, "rgss_main", mriRgssMain);
        _rb_define_module_function(rb_mKernel, "rgss_stop", mriRgssStop);

        _rb_define_module_function(rb_mKernel, "msgbox", mriPrint);
        _rb_define_module_function(rb_mKernel, "msgbox_p", mriP);

        rb_define_global_const("RGSS_VERSION", rb_utf8_str_new_cstr("3.0.1"));
    } else {
        _rb_define_module_function(rb_mKernel, "print", mriPrint);
        _rb_define_module_function(rb_mKernel, "p", mriP);

        rb_define_alias(rb_singleton_class(rb_mKernel), "_mkshot_kernel_caller_alias",
                        "caller");
        _rb_define_module_function(rb_mKernel, "caller", _kernelCaller);
    }

    if (rgssVer == 1)
        rb_eval_string(module_rgss1);
    else if (rgssVer == 2)
        rb_eval_string(module_rgss2);
    else if (rgssVer == 3)
        rb_eval_string(module_rgss3);
    else
        assert(!"unreachable");

    VALUE mod = rb_define_module("System");
    _rb_define_module_function(mod, "delta", mkshotDelta);
    _rb_define_module_function(mod, "uptime", mkshotDelta);
    _rb_define_module_function(mod, "data_directory", mkshotDataDirectory);
    _rb_define_module_function(mod, "set_window_title", mkshotSetTitle);
    _rb_define_module_function(mod, "window_title", mkshotGetTitle);
    _rb_define_module_function(mod, "window_title=", mkshotSetTitle);
    _rb_define_module_function(mod, "show_settings", mkshotSettingsMenu);
    _rb_define_module_function(mod, "puts", mkshotPuts);
    _rb_define_module_function(mod, "desensitize", mkshotDesensitize);
    _rb_define_module_function(mod, "platform", mkshotPlatform);

    _rb_define_module_function(mod, "is_mac?", mkshotIsMacHost);
    _rb_define_module_function(mod, "is_rosetta?", mkshotIsUsingRosetta);

    _rb_define_module_function(mod, "is_linux?", mkshotIsLinuxHost);

    _rb_define_module_function(mod, "is_windows?", mkshotIsWindowsHost);
    _rb_define_module_function(mod, "is_wine?", mkshotIsUsingWine);
    _rb_define_module_function(mod, "is_really_mac?", mkshotIsReallyMacHost);
    _rb_define_module_function(mod, "is_really_linux?", mkshotIsReallyLinuxHost);
    _rb_define_module_function(mod, "is_really_windows?", mkshotIsReallyWindowsHost);

    _rb_define_module_function(mod, "user_language", mkshotUserLanguage);
    _rb_define_module_function(mod, "user_name", mkshotUserName);
    _rb_define_module_function(mod, "user_fullname", mkshotUserFullName);

    _rb_define_module_function(mod, "game_title", mkshotGameTitle);
    _rb_define_module_function(mod, "power_state", mkshotPowerState);
    _rb_define_module_function(mod, "nproc", mkshotCpuCount);
    _rb_define_module_function(mod, "memory", mkshotSystemMemory);
    _rb_define_module_function(mod, "reload_cache", mkshotReloadPathCache);
    _rb_define_module_function(mod, "mount", mkshotAddPath);
    _rb_define_module_function(mod, "unmount", mkshotRemovePath);
    _rb_define_module_function(mod, "file_exist?", mkshotFileExists);
    _rb_define_module_function(mod, "launch", mkshotLaunch);

    _rb_define_module_function(mod, "default_font_family=", mkshotSetDefaultFontFamily);



    VALUE cmod = rb_define_module("CFG");
    _rb_define_module_function(cmod, "[]", mkshotGetJSONSetting);
    _rb_define_module_function(cmod, "[]=", mkshotSetJSONSetting);
    _rb_define_module_function(cmod, "to_hash", mkshotGetAllJSONSettings);

    /* Load global constants */
    rb_gv_set("MKSHOT", Qtrue);

    VALUE debug = rb_bool_new(shState->config().editor.debug);
    if (rgssVer == 1)
        rb_gv_set("DEBUG", debug);
    else if (rgssVer >= 2)
        rb_gv_set("TEST", debug);

    rb_gv_set("BTEST", rb_bool_new(shState->config().editor.battleTest));

    /* OneShot also uses $debug in lowercase */
    if (shState->config().editor.debug || shState->config().debugMode)
        rb_gv_set("debug", Qtrue);
    else
        rb_gv_set("debug", Qfalse);

    /* Set mkshot-z git hash constants */
    std::string ver_hash;

#ifdef __APPLE__
    ver_hash = getPlistValue("MKShotVerHash");
#elif defined(MKSHOT_VER_HASH)
    ver_hash = MKSHOT_VER_HASH;
#endif

    // fallback value is already set by the build system
    // but let it be like that
    if (ver_hash.empty()) {
        ver_hash = "no hash found :p";
    }

    VALUE rbstr_ver_hash = rb_utf8_str_new_cstr(ver_hash.c_str());
    rb_str_freeze(rbstr_ver_hash);
    rb_define_const(mod, "VER_HASH", rbstr_ver_hash);

    // Set $stdout and its ilk accordingly on Windows
    // I regret teaching you that word
#ifdef __WIN32__
    if (shState->config().winConsole)
        configureWindowsStreams();
#endif
}

static void showMsg(const std::string &msg) {
    shState->eThread().showMessageBox(msg.c_str());
}

static void printP(int argc, VALUE *argv, const char *convMethod,
                   const char *sep) {
    VALUE dispString = rb_str_buf_new(128);
    ID conv = rb_intern(convMethod);

    for (int i = 0; i < argc; ++i) {
        VALUE str = rb_funcall2(argv[i], conv, 0, NULL);
        rb_str_buf_append(dispString, str);

        if (i < argc)
            rb_str_buf_cat2(dispString, sep);
    }

    showMsg(RSTRING_PTR(dispString));
}


RB_METHOD(mriPrint) {
    RB_UNUSED_PARAM;

    printP(argc, argv, "to_s", "");

    return Qnil;
}

RB_METHOD(mriP) {
    RB_UNUSED_PARAM;

    printP(argc, argv, "inspect", "\n");

    return Qnil;
}

RB_METHOD(mkshotDelta) {
    RB_UNUSED_PARAM;
    return rb_float_new(shState->runTime());
}

RB_METHOD(mkshotDataDirectory) {
    RB_UNUSED_PARAM;

    const std::string &path = shState->config().customDataPath;
    const char *s = path.empty() ? "." : path.c_str();

    std::string s_nml = shState->fileSystem().normalize(s, 1, 1);
    VALUE ret = rb_utf8_str_new_cstr(s_nml.c_str());

    return ret;
}

RB_METHOD(mkshotSetTitle) {
    RB_UNUSED_PARAM;

    VALUE s;
    rb_scan_args(argc, argv, "1", &s);
    SafeStringValue(s);

    shState->eThread().requestWindowRename(RSTRING_PTR(s));
    return s;
}

RB_METHOD(mkshotGetTitle) {
    RB_UNUSED_PARAM;

    rb_check_argc(argc, 0);

    return rb_utf8_str_new_cstr(SDL_GetWindowTitle(shState->sdlWindow()));
}

RB_METHOD(mkshotDesensitize) {
    RB_UNUSED_PARAM;

    VALUE filename;
    rb_scan_args(argc, argv, "1", &filename);
    SafeStringValue(filename);

    return rb_utf8_str_new_cstr(
                                shState->fileSystem().desensitize(RSTRING_PTR(filename)));
}

RB_METHOD(mkshotPuts) {
    RB_UNUSED_PARAM;

    const char *str;
    rb_get_args(argc, argv, "z", &str RB_ARG_END);

    Debug() << str;

    return Qnil;
}

RB_METHOD(mkshotPlatform) {
    RB_UNUSED_PARAM;

#ifdef __APPLE__
    std::string platform("macOS");

    if (mkshot_sys::isRosetta())
        platform += " (Rosetta)";

#elif defined __WIN32__
    std::string platform("Windows");

    if (mkshot_sys::isWine()) {
        platform += " (Wine - ";
        switch (mkshot_sys::getRealHostType()) {
            case mkshot_sys::WineHostType::Mac:
                platform += "macOS)";
                break;
            default:
                platform += "Linux)";
                break;
        }
    }
#elif defined __linux__
    std::string platform("Linux");
#endif

    return rb_utf8_str_new_cstr(platform.c_str());
}

// TODO: get rid of this nonsense
RB_METHOD(mkshotIsMacHost) {
    RB_UNUSED_PARAM;

#ifdef __APPLE__
    return rb_bool_new(true);
#else
    return rb_bool_new(false);
#endif
}

RB_METHOD(mkshotIsUsingRosetta) {
    RB_UNUSED_PARAM;

    return rb_bool_new(mkshot_sys::isRosetta());
}

RB_METHOD(mkshotIsLinuxHost) {
    RB_UNUSED_PARAM;

#ifdef __linux__
    return rb_bool_new(true);
#else
    return rb_bool_new(false);
#endif
}

RB_METHOD(mkshotIsWindowsHost) {
    RB_UNUSED_PARAM;

#ifdef __WIN32__
    return rb_bool_new(true);
#else
    return rb_bool_new(false);
#endif
}

RB_METHOD(mkshotIsUsingWine) {
    RB_UNUSED_PARAM;
    return rb_bool_new(mkshot_sys::isWine());
}

RB_METHOD(mkshotIsReallyMacHost) {
    RB_UNUSED_PARAM;
    return rb_bool_new(mkshot_sys::getRealHostType() == mkshot_sys::WineHostType::Mac);
}

RB_METHOD(mkshotIsReallyLinuxHost) {
    RB_UNUSED_PARAM;
    return rb_bool_new(mkshot_sys::getRealHostType() == mkshot_sys::WineHostType::Linux);
}

RB_METHOD(mkshotIsReallyWindowsHost) {
    RB_UNUSED_PARAM;
    return rb_bool_new(mkshot_sys::getRealHostType() == mkshot_sys::WineHostType::Windows);
}

RB_METHOD(mkshotUserLanguage) {
    RB_UNUSED_PARAM;

    return rb_utf8_str_new_cstr(mkshot_sys::getLanguage().c_str());
}

RB_METHOD(mkshotUserName) {
    RB_UNUSED_PARAM;

    return rb_utf8_str_new_cstr(mkshot_sys::getUserName().c_str());
}

RB_METHOD(mkshotUserFullName) {
    RB_UNUSED_PARAM;

    return rb_utf8_str_new_cstr(mkshot_sys::getUserFullName().c_str());
}

RB_METHOD(mkshotGameTitle) {
    RB_UNUSED_PARAM;

    return rb_utf8_str_new_cstr(shState->config().game.title.c_str());
}

RB_METHOD(mkshotPowerState) {
    RB_UNUSED_PARAM;

    int secs, pct;
    SDL_PowerState ps = SDL_GetPowerInfo(&secs, &pct);

    VALUE hash = rb_hash_new();

    rb_hash_aset(hash, ID2SYM(rb_intern("seconds")),
                 (secs > -1) ? INT2NUM(secs) : RUBY_Qnil);

    rb_hash_aset(hash, ID2SYM(rb_intern("percent")),
                 (pct > -1) ? INT2NUM(pct) : RUBY_Qnil);

    rb_hash_aset(hash, ID2SYM(rb_intern("discharging")),
                 rb_bool_new(ps == SDL_POWERSTATE_ON_BATTERY));

    return hash;
}

RB_METHOD(mkshotSettingsMenu) {
    RB_UNUSED_PARAM;

    shState->eThread().requestSettingsMenu();

    return Qnil;
}

RB_METHOD(mkshotCpuCount) {
    RB_UNUSED_PARAM;

    return INT2NUM(SDL_GetCPUCount());
}

RB_METHOD(mkshotSystemMemory) {
    RB_UNUSED_PARAM;

    return INT2NUM(SDL_GetSystemRAM());
}

RB_METHOD(mkshotReloadPathCache) {
    RB_UNUSED_PARAM;

    GUARD_EXC(shState->fileSystem().reloadPathCache(););
    return Qnil;
}

RB_METHOD(mkshotAddPath) {
    RB_UNUSED_PARAM;

    VALUE path, mountpoint, reload;
    rb_scan_args(argc, argv, "12", &path, &mountpoint, &reload);
    SafeStringValue(path);
    if (mountpoint != Qnil) SafeStringValue(mountpoint);

    const char *mp = (mountpoint == Qnil) ? 0 : RSTRING_PTR(mountpoint);

    try {
        bool rl = true;
        if (reload != Qnil)
            rb_bool_arg(reload, &rl);

        shState->fileSystem().addPath(RSTRING_PTR(path), mp, rl);
    } catch (Exception &e) {
        raiseRbExc(e);
    }
    return path;
}

RB_METHOD(mkshotRemovePath) {
    RB_UNUSED_PARAM;

    VALUE path, reload;
    rb_scan_args(argc, argv, "11", &path, &reload);
    SafeStringValue(path);

    try {
        bool rl = true;
        if (reload != Qnil)
            rb_bool_arg(reload, &rl);

        shState->fileSystem().removePath(RSTRING_PTR(path), rl);
    } catch (Exception &e) {
        raiseRbExc(e);
    }
    return path;
}

RB_METHOD(mkshotFileExists) {
    RB_UNUSED_PARAM;

    VALUE path;
    rb_scan_args(argc, argv, "1", &path);
    SafeStringValue(path);

    if (shState->fileSystem().exists(RSTRING_PTR(path)))
        return Qtrue;
    return Qfalse;
}

RB_METHOD(mkshotSetDefaultFontFamily) {
    RB_UNUSED_PARAM;

    VALUE familyV;
    rb_scan_args(argc, argv, "1", &familyV);
    SafeStringValue(familyV);

    std::string family(RSTRING_PTR(familyV));
    shState->fontState().setDefaultFontFamily(family);

    return Qnil;
}

#ifdef __APPLE__
#define OPENCMD "open "
#define OPENARGS "--args"
#elif defined(__linux__)
#define OPENCMD "xdg-open "
#define OPENARGS ""
#else
#define OPENCMD "start /b \"launch\" "
#define OPENARGS ""
#endif

RB_METHOD(mkshotLaunch) {
    RB_UNUSED_PARAM;

    VALUE cmdname, args;

    rb_scan_args(argc, argv, "11", &cmdname, &args);
    SafeStringValue(cmdname);

    std::string command(OPENCMD);
    command += "\""; command += RSTRING_PTR(cmdname); command += "\"";

    if (args != RUBY_Qnil) {
#ifndef __linux__
        command += " ";
        command += OPENARGS;
        Check_Type(args, T_ARRAY);

        for (int i = 0; i < RARRAY_LEN(args); i++) {
            VALUE arg = rb_ary_entry(args, i);
            SafeStringValue(arg);

            if (RSTRING_LEN(arg) <= 0)
                continue;

            command += " ";
            command += RSTRING_PTR(arg);
        }
#else
        Debug() << command << ":" << "Arguments are not supported with xdg-open. Ignoring.";
#endif
    }

    if (std::system(command.c_str()) != 0) {
        raiseRbExc(Exception(Exception::MKShotError, "Failed to launch \"%s\"", RSTRING_PTR(cmdname)));
    }

    return RUBY_Qnil;
}

json5pp::value loadUserSettings() {
    json5pp::value ret;
    VALUE cpath = rb_utf8_str_new_cstr(shState->config().userConfPath.c_str());

    if (rb_funcall(rb_cFile, rb_intern("exists?"), 1, cpath) == Qtrue) {
        VALUE f = rb_funcall(rb_cFile, rb_intern("open"), 2, cpath, rb_str_new("r", 1));
        VALUE data = rb_funcall(f, rb_intern("read"), 0);
        rb_funcall(f, rb_intern("close"), 0);
        ret = json5pp::parse5(RSTRING_PTR(data));
    }

    if (!ret.is_object())
        ret = json5pp::object({});

    return ret;
}

void saveUserSettings(json5pp::value &settings) {
    VALUE cpath = rb_utf8_str_new_cstr(shState->config().userConfPath.c_str());
    VALUE f = rb_funcall(rb_cFile, rb_intern("open"), 2, cpath, rb_str_new("w", 1));
    rb_funcall(f, rb_intern("write"), 1, rb_utf8_str_new_cstr(settings.stringify5(json5pp::rule::space_indent<>()).c_str()));
    rb_funcall(f, rb_intern("close"), 0);
}

RB_METHOD(mkshotGetJSONSetting) {
    RB_UNUSED_PARAM;

    VALUE sname;
    rb_scan_args(argc, argv, "1", &sname);
    SafeStringValue(sname);

    auto settings = loadUserSettings();
    auto &s = settings.as_object();

    if (s[RSTRING_PTR(sname)].is_null()) {
        return json2rb(shState->config().raw.as_object()[RSTRING_PTR(sname)]);
    }

    return json2rb(s[RSTRING_PTR(sname)]);

}

RB_METHOD(mkshotSetJSONSetting) {
    RB_UNUSED_PARAM;

    VALUE sname, svalue;
    rb_scan_args(argc, argv, "2", &sname, &svalue);
    SafeStringValue(sname);

    auto settings = loadUserSettings();
    auto &s = settings.as_object();
    s[RSTRING_PTR(sname)] = rb2json(svalue);
    saveUserSettings(settings);

    return Qnil;
}

RB_METHOD(mkshotGetAllJSONSettings) {
    RB_UNUSED_PARAM;

    return json2rb(shState->config().raw);
}

static VALUE rgssMainCb(VALUE block) {
    rb_funcall2(block, rb_intern("call"), 0, 0);
    return Qnil;
}

static VALUE rgssMainRescue(VALUE arg, VALUE exc) {
    VALUE *excRet = (VALUE *)arg;

    *excRet = exc;

    return Qnil;
}

static void processReset() {
    shState->graphics().reset();
    shState->audio().reset();

    shState->rtData().rqReset.clear();
    shState->graphics().repaintWait(shState->rtData().rqResetFinish, false);
}

RB_METHOD(mriRgssMain) {
    RB_UNUSED_PARAM;

    while (true) {
        VALUE exc = Qnil;
        rb_rescue2(rgssMainCb, rb_block_proc(), rgssMainRescue, (VALUE)&exc,
                   rb_eException, (VALUE)0);

        if (NIL_P(exc))
            break;

        if (rb_obj_class(exc) == getRbData()->exc[Reset])
            processReset();
        else
            rb_exc_raise(exc);
    }

    return Qnil;
}

RB_METHOD(mriRgssStop) {
    RB_UNUSED_PARAM;

    while (true)
        shState->graphics().update();

    return Qnil;
}

RB_METHOD(_kernelCaller) {
    RB_UNUSED_PARAM;

    VALUE trace =
    rb_funcall2(rb_mKernel, rb_intern("_mkshot_kernel_caller_alias"), 0, 0);

    if (!RB_TYPE_P(trace, RUBY_T_ARRAY))
        return trace;

    long len = RARRAY_LEN(trace);

    if (len < 2)
        return trace;

    /* Remove useless "ruby:1:in 'eval'" */
    rb_ary_pop(trace);

    /* Also remove trace of this helper function */
    rb_ary_shift(trace);

    len -= 2;

    if (len == 0)
        return trace;

    /* RMXP does this, not sure if specific or 1.8 related */
    VALUE args[] = {rb_utf8_str_new_cstr(":in `<main>'"), rb_utf8_str_new_cstr("")};
    rb_funcall2(rb_ary_entry(trace, len - 1), rb_intern("gsub!"), 2, args);

    return trace;
}

static VALUE newStringUTF8(const char *string, long length) {
    return rb_enc_str_new(string, length, rb_utf8_encoding());
}

struct evalArg {
    VALUE string;
    VALUE filename;
};

static VALUE evalHelper(evalArg *arg) {
    VALUE argv[] = {arg->string, Qnil, arg->filename};
    return rb_funcall2(Qnil, rb_intern("eval"), ARRAY_SIZE(argv), argv);
}

static VALUE evalString(VALUE string, VALUE filename, int *state) {
    evalArg arg = {string, filename};
    return rb_protect((VALUE(*)(VALUE))evalHelper, (VALUE)&arg, state);
}

static void runCustomScript(const std::string &filename) {
    std::string scriptData;

    if (!readFileSDL(filename.c_str(), scriptData)) {
        showMsg(std::string("Unable to open '") + filename + "'");
        return;
    }

    evalString(newStringUTF8(scriptData.c_str(), scriptData.size()),
               newStringUTF8(filename.c_str(), filename.size()), NULL);
}

VALUE kernelLoadDataInt(const char *filename, bool rubyExc, bool raw);

struct BacktraceData {
    /* Maps: Ruby visible filename, To: Actual script name */
    BoostHash<std::string, std::string> scriptNames;
};

bool evalScript(VALUE string, const char *filename)
{
    int state;
    evalString(string, rb_utf8_str_new_cstr(filename), &state);
    if (state) return false;
    return true;
}


#define SCRIPT_SECTION_FMT (rgssVer >= 3 ? "{%04ld}" : "Section%03ld")

static void runRMXPScripts(BacktraceData &btData) {
    const Config &conf = shState->rtData().config;
    const std::string &scriptPack = conf.game.scripts;

    if (scriptPack.empty()) {
        showMsg("No script file has been specified. Check the game's INI and try again.");
        return;
    }

    if (!shState->fileSystem().exists(scriptPack.c_str())) {
        showMsg("Unable to load scripts from '" + scriptPack + "'");
        return;
    }

    VALUE scriptArray;

    /* We checked if Scripts.rxdata exists, but something might
     * still go wrong */
    try {
        scriptArray = kernelLoadDataInt(scriptPack.c_str(), false, false);
    } catch (const Exception &e) {
        showMsg(std::string("Failed to read script data: ") + e.msg);
        return;
    }

    if (!RB_TYPE_P(scriptArray, RUBY_T_ARRAY)) {
        showMsg("Failed to read script data");
        return;
    }

    rb_gv_set("$RGSS_SCRIPTS", scriptArray);

    long scriptCount = RARRAY_LEN(scriptArray);

    std::string decodeBuffer;
    decodeBuffer.resize(0x1000);

    for (long i = 0; i < scriptCount; ++i) {
        VALUE script = rb_ary_entry(scriptArray, i);

        if (!RB_TYPE_P(script, RUBY_T_ARRAY))
            continue;

        VALUE scriptName = rb_ary_entry(script, 1);
        VALUE scriptString = rb_ary_entry(script, 2);

        int result = Z_OK;
        unsigned long bufferLen;

        while (true) {
            unsigned char *bufferPtr = reinterpret_cast<unsigned char *>(
                                                                         const_cast<char *>(decodeBuffer.c_str()));
            const unsigned char *sourcePtr =
            reinterpret_cast<const unsigned char *>(RSTRING_PTR(scriptString));

            bufferLen = decodeBuffer.length();

            result = uncompress(bufferPtr, &bufferLen, sourcePtr,
                                RSTRING_LEN(scriptString));

            bufferPtr[bufferLen] = '\0';

            if (result != Z_BUF_ERROR)
                break;

            decodeBuffer.resize(decodeBuffer.size() * 2);
        }

        if (result != Z_OK) {
            static char buffer[256];
            snprintf(buffer, sizeof(buffer), "Error decoding script %ld: '%s'", i,
                     RSTRING_PTR(scriptName));

            showMsg(buffer);

            break;
        }

        rb_ary_store(script, 3, rb_utf8_str_new_cstr(decodeBuffer.c_str()));
    }

    /* Execute preloaded scripts */
    for (std::vector<std::string>::const_iterator i = conf.preloadScripts.begin();
         i != conf.preloadScripts.end(); ++i)
    {
        if (shState->rtData().rqTerm)
            break;
        runCustomScript(*i);
    }

    VALUE exc = rb_gv_get("$!");
    if (exc != Qnil)
        return;

    while (true) {
        for (long i = 0; i < scriptCount; ++i) {
            if (shState->rtData().rqTerm)
                break;

            VALUE script = rb_ary_entry(scriptArray, i);
            VALUE scriptDecoded = rb_ary_entry(script, 3);
            VALUE string =
            newStringUTF8(RSTRING_PTR(scriptDecoded), RSTRING_LEN(scriptDecoded));

            VALUE fname;
            const char *scriptName = RSTRING_PTR(rb_ary_entry(script, 1));
            char buf[512];
            int len;

            if (conf.useScriptNames)
                len = snprintf(buf, sizeof(buf), "%03ld:%s", i, scriptName);
            else
                len = snprintf(buf, sizeof(buf), SCRIPT_SECTION_FMT, i);

            fname = newStringUTF8(buf, len);
            btData.scriptNames.insert(buf, scriptName);


            // if the script name starts with |s|, only execute
            // it if "s" is the same first letter as the platform
            // we're running on

            // |W| - Windows, |M| - Mac OS X, |L| - Linux

            // Adding a 'not' symbol means it WON'T run on that
            // platform (i.e. |!W| won't run on Windows)
            /*
             if (scriptName[0] == '|') {
             int len = strlen(scriptName);
             if (len > 2) {
             if (scriptName[1] == '!' && len > 3 &&
             scriptName[3] == scriptName[0]) {
             if (toupper(scriptName[2]) == platform[0])
             continue;
             }
             if (scriptName[2] == scriptName[0] &&
             toupper(scriptName[1]) != platform[0])
             continue;
             }
             }
             */

            int state;

            evalString(string, fname, &state);
            if (state)
                break;
        }

        VALUE exc = rb_gv_get("$!");
        if (rb_obj_class(exc) != getRbData()->exc[Reset])
            break;

        processReset();
    }
}

static void showExc(VALUE exc, const BacktraceData &btData) {
    VALUE bt = rb_funcall2(exc, rb_intern("backtrace"), 0, NULL);
    VALUE msg = rb_funcall2(exc, rb_intern("message"), 0, NULL);
    VALUE bt0 = rb_ary_entry(bt, 0);
    VALUE name = rb_class_path(rb_obj_class(exc));

    VALUE ds = rb_sprintf("%" PRIsVALUE ": %" PRIsVALUE " (%" PRIsVALUE ")", bt0, exc, name);

    /* omit "useless" last entry (from ruby:1:in `eval') */
    for (long i = 1, btlen = RARRAY_LEN(bt) - 1; i < btlen; ++i)
        rb_str_catf(ds, "\n\tfrom %" PRIsVALUE,
                    rb_ary_entry(bt, i));
    Debug() << StringValueCStr(ds);

    char *s = RSTRING_PTR(bt0);

    char line[16];
    std::string file(512, '\0');

    char *p = s + strlen(s);
    char *e;

    while (p != s)
        if (*--p == ':')
            break;

    e = p;

    while (p != s)
        if (*--p == ':')
            break;

    /* s         p  e
     * SectionXXX:YY: in 'blabla' */

    *e = '\0';
    strncpy(line, *p ? p + 1 : p, sizeof(line));
    line[sizeof(line) - 1] = '\0';
    *e = ':';
    e = p;

    /* s         e
     * SectionXXX:YY: in 'blabla' */

    *e = '\0';
    strncpy(&file[0], s, file.size());
    *e = ':';

    /* Shrink to fit */
    file.resize(strlen(file.c_str()));
    file = btData.scriptNames.value(file, file);

    std::string ms(640, '\0');
    snprintf(&ms[0], ms.size(), "Script '%s' line %s: %s occured.\n\n%s",
             file.c_str(), line, RSTRING_PTR(name), RSTRING_PTR(msg));

    showMsg(ms);
}

static void mriBindingExecute() {
    Config &conf = shState->rtData().config;

    /* Normally only a ruby executable would do a sysinit,
     * but not doing it will lead to crashes due to closed
     * stdio streams on some platforms (eg. Windows) */
    int argc = 0;
    char **argv = 0;
    ruby_sysinit(&argc, &argv);

    RUBY_INIT_STACK;
    ruby_init();

    std::vector<const char *> rubyArgsC {"mkshot"};
    rubyArgsC.push_back("-e ");
    void *node;
    if (conf.jit.enabled) {
        std::string verboseLevel("--mjit-verbose=");
        std::string maxCache("--mjit-max-cache=");
        std::string minCalls("--mjit-min-calls=");
        rubyArgsC.push_back("--mjit");
        verboseLevel += std::to_string(conf.jit.verboseLevel);
        maxCache += std::to_string(conf.jit.maxCache);
        minCalls += std::to_string(conf.jit.minCalls);

        rubyArgsC.push_back(verboseLevel.c_str());
        rubyArgsC.push_back(maxCache.c_str());
        rubyArgsC.push_back(minCalls.c_str());
        node = ruby_options(rubyArgsC.size(), const_cast<char**>(rubyArgsC.data()));
    } else if (conf.yjit.enabled) {
        rubyArgsC.push_back("--yjit");
        // TODO: Maybe support --yjit-exec-mem-size, --yjit-call-threshold
        node = ruby_options(rubyArgsC.size(), const_cast<char**>(rubyArgsC.data()));
    } else {
        node = ruby_options(rubyArgsC.size(), const_cast<char**>(rubyArgsC.data()));
    }

    int state;
    bool valid = ruby_executable_node(node, &state);
    if (valid)
        state = ruby_exec_node(node);
    if (state || !valid) {
        // The message is formatted for and automatically spits
        // out to the terminal, so let's leave it that way for now
        /*
         VALUE exc = rb_errinfo();
         VALUE msg = rb_funcall(exc, rb_intern("full_message"), 0);
         */
        showMsg("An error occurred while initializing Ruby. (Invalid JIT settings?)");
        ruby_cleanup(state);
        shState->rtData().rqTermAck.set();
        return;
    }
    rb_enc_set_default_internal(rb_enc_from_encoding(rb_utf8_encoding()));
    rb_enc_set_default_external(rb_enc_from_encoding(rb_utf8_encoding()));

    VALUE rbArgv = rb_get_argv();
    for (const auto &str : conf.launchArgs)
        rb_ary_push(rbArgv, rb_utf8_str_new_cstr(str.c_str()));

    // Duplicates get pushed for some reason
    rb_funcall(rbArgv, rb_intern("uniq!"), 0);

    VALUE lpaths = rb_gv_get(":");
    rb_ary_clear(lpaths);

#ifdef __APPLE__
    std::string resPath = mkshot_fs::getResourcePath();
    resPath += "/Ruby/" + std::to_string(RUBY_API_VERSION_MAJOR) + "." + std::to_string(RUBY_API_VERSION_MINOR) + ".0";
    rb_ary_push(lpaths, rb_str_new(resPath.c_str(), resPath.size()));
#endif

    if (!conf.rubyLoadpaths.empty()) {
        /* Setup custom load paths */
        for (size_t i = 0; i < conf.rubyLoadpaths.size(); ++i) {
            std::string &path = conf.rubyLoadpaths[i];

            VALUE pathv = rb_str_new(path.c_str(), path.size());
            rb_ary_push(lpaths, pathv);
        }
    }
#ifndef KEEP_CWD
    else {
        rb_ary_push(lpaths, rb_utf8_str_new_cstr(mkshot_fs::getCurrentDirectory().c_str()));
    }
#endif

    RbData rbData;
    shState->setBindingData(&rbData);
    BacktraceData btData;

    mriBindingInit();

    std::string &customScript = conf.customScript;
    if (!customScript.empty())
        runCustomScript(customScript);
    else
        runRMXPScripts(btData);

    VALUE exc = rb_errinfo();
    if (!NIL_P(exc) && !rb_obj_is_kind_of(exc, rb_eSystemExit))
        showExc(exc, btData);

    ruby_cleanup(0);

    // Force allow exit
    shState->rtData().allowExit.set();

    // Request EventThread termination
    shState->rtData().rqTermAck.set();
}

static void mriBindingTerminate() { rb_raise(rb_eSystemExit, " "); }

static void mriBindingReset() { rb_raise(getRbData()->exc[Reset], " "); }
