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

#include "core/oneshot/oneshot-apple.hpp"

#include <string>

#import <Foundation/Foundation.h>
#import <AppKit/AppKit.h>

BOOL isCached = NO;
NSURL *cachedImageURL;
NSDictionary<NSWorkspaceDesktopImageOptionKey, id> *cachedImageOptions;

NSScreen *screen = [NSScreen mainScreen];
NSWorkspace *sharedworkspace = [NSWorkspace sharedWorkspace];

void OneshotApple::desktopImageSet(std::string path, double red, double green, double blue)
{
    NSURL *url = [NSURL fileURLWithPath:@(path.c_str())];
    NSDictionary<NSWorkspaceDesktopImageOptionKey, id> *options = @{
        NSWorkspaceDesktopImageScalingKey : @3,
        NSWorkspaceDesktopImageAllowClippingKey : @0,
        NSWorkspaceDesktopImageFillColorKey : [NSColor colorWithSRGBRed:red green:green blue:blue alpha:1.0]
    };

    NSError *error = nil;
    BOOL success = [sharedworkspace setDesktopImageURL:[url absoluteURL] forScreen:screen options:options error:&error];
    if (!success)
        NSLog(@"Failure to set new desktop image: %@", error);
}

void OneshotApple::desktopImageReset()
{
    if (!isCached)
        return;

    NSError *error = nil;
    BOOL success = [sharedworkspace setDesktopImageURL:cachedImageURL forScreen:screen options:cachedImageOptions error:&error];
    if (!success)
        NSLog(@"Failure to set old desktop image: %@", error);
}

void OneshotApple::desktopImageCache()
{
    if (isCached)
        return;

    cachedImageURL = [sharedworkspace desktopImageURLForScreen:screen];
    cachedImageOptions = [sharedworkspace desktopImageOptionsForScreen:screen];
    isCached = YES;
}
