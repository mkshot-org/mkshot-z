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

#import "core/sys/sys.hpp"

#import <AppKit/AppKit.h>
#import <Metal/Metal.h>
#import <sys/sysctl.h>

/* TODO: settings menu rewrite
#import "SettingsMenuController.hpp"
*/

std::string mkshot_sys::getLanguage()
{
    NSString *languageCode = NSLocale.currentLocale.languageCode;
    NSString *countryCode = NSLocale.currentLocale.countryCode;
    return std::string([NSString stringWithFormat:@"%@_%@", languageCode, countryCode].UTF8String);
}

std::string mkshot_sys::getUserName()
{
    NSString *name = NSUserName();
    return std::string(name.UTF8String);
}

std::string mkshot_sys::getUserFullName()
{
    NSString *name = NSFullUserName();

    if ([name isEqualToString:@""])
        name = NSUserName();

    return std::string(name.UTF8String);
}

int mkshot_sys::getScalingFactor()
{
    return NSApplication.sharedApplication.mainWindow.backingScaleFactor;
}

bool mkshot_sys::isWine()
{
    // Always false on Mac builds, see sys.cpp
    return false;
}

bool mkshot_sys::isRosetta()
{
    int translated = 0;
    size_t size = sizeof(translated);

    int result = sysctlbyname("sysctl.proc_translated", &translated, &size, NULL, 0);

    if (result == -1)
        return false;

    return translated;
}

mkshot_sys::WineHostType mkshot_sys::getRealHostType()
{
    return WineHostType::Mac;
}

// Constant, if it's not nil then just raise the menu instead.
SettingsMenu *smenu = nil;

void openSettingsWindow()
{
    if (smenu == nil) {
        smenu = [SettingsMenu openWindow];
        return;
    }
    [smenu raise];
}

bool isMetalSupported()
{
    if (@available(macOS 10.13.0, *))
        return MTLCreateSystemDefaultDevice() != nil;
    return false;
}

std::string getPlistValue(const char *key)
{
    NSString *hash = [[NSBundle mainBundle] objectForInfoDictionaryKey:@(key)];
    if (hash != nil)
        return std::string(hash.UTF8String);
    return "";
}
