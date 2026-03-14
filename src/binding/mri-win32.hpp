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

#pragma once

#include <windows.h>
#include "util/win-console.hpp"

// Attempts to set $stdout and $stdin accordingly on Windows. Only
// called when debug mode is on, since that's when the console
// should be active.
void configureWindowsStreams() {
    const int stdoutFD = getStdFD(STD_OUTPUT_HANDLE);

    // Configure $stdout
    if (stdoutFD >= 0) {
        VALUE winStdout = rb_funcall(rb_cIO, rb_intern("new"), 2,
            INT2NUM(stdoutFD), rb_str_new_cstr("w+"));

        rb_gv_set("stdout", winStdout);
    }

    const int stdinFD = getStdFD(STD_INPUT_HANDLE);

    // Configure $stdin
    if (stdinFD >= 0) {
        VALUE winStdin = rb_funcall(rb_cIO, rb_intern("new"), 2,
            INT2NUM(stdinFD), rb_str_new_cstr("r"));

        rb_gv_set("stdin", winStdin);
    }

    const int stderrFD = getStdFD(STD_ERROR_HANDLE);

    // Configure $stderr
    if (stderrFD >= 0) {
        VALUE winStderr = rb_funcall(rb_cIO, rb_intern("new"), 2,
            INT2NUM(stderrFD), rb_str_new_cstr("w+"));

        rb_gv_set("stderr", winStderr);
    }
}

#endif
