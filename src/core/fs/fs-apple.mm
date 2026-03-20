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

#import <AppKit/AppKit.h>
#import <SDL3/SDL_syswm.h>
#import <SDL3/SDL_filesystem.h>

#import "core/fs/fs.hpp"
#import "util/exception.hpp"

#define PATHTONS(str) [NSFileManager.defaultManager stringWithFSRepresentation:str length:strlen(str)]
#define NSTOPATH(str) [NSFileManager.defaultManager fileSystemRepresentationWithPath:str]

bool mkshot_fs::fileExists(const char *path)
{
    BOOL isDir;
    return [NSFileManager.defaultManager fileExistsAtPath:PATHTONS(path) isDirectory:&isDir] && !isDir;
}

std::string mkshot_fs::contentsOfFileAsString(const char *path)
{
    NSStringEncoding enc;

    NSString *fileContents = [NSString stringWithContentsOfFile:PATHTONS(path) usedEncoding:&enc error:NULL];

    if (fileContents == nil)
        throw Exception(Exception::NoFileError, "Failed to read file at %s", path);

    return std::string(fileContents.UTF8String);
}

bool mkshot_fs::setCurrentDirectory(const char *path)
{
    return [NSFileManager.defaultManager changeCurrentDirectoryPath:PATHTONS(path)];
}

std::string mkshot_fs::getCurrentDirectory()
{
    return std::string(NSTOPATH(NSFileManager.defaultManager.currentDirectoryPath));
}

std::string mkshot_fs::normalizePath(const char *path, bool preferred, bool absolute)
{
    NSString *nsPathOrig = PATHTONS(path);
    NSString *nsPath = [NSURL fileURLWithPath:nsPathOrig].URLByStandardizingPath.path;
    NSString *pwd = [NSString stringWithFormat:@"%@/", NSFileManager.defaultManager.currentDirectoryPath];

    if ([nsPathOrig hasSuffix:@"/"])
        nsPath = [nsPath stringByAppendingString:@"/"];

    if (!absolute)
        nsPath = [nsPath stringByReplacingOccurrencesOfString:pwd withString:@""];

    nsPath = [nsPath stringByReplacingOccurrencesOfString:@"\\" withString:@"/"];

    return std::string(NSTOPATH(nsPath));
}

std::string mkshot_fs::getDefaultGameRoot()
{
    NSString *path = [NSString stringWithFormat:@"%@/../", NSBundle.mainBundle.bundlePath];
    return normalizePath(NSTOPATH(path), true, true);
}

NSString *getPathForAsset_internal(const char *baseName, const char *ext)
{
    NSString *pathBundle = [NSString stringWithFormat:@"%@/%s", NSBundle.mainBundle.resourcePath, "Assets.bundle"];
    NSBundle *assetBundle = [NSBundle bundleWithPath:pathBundle];

    if (assetBundle == nil)
        return nil;

    return [assetBundle pathForResource:@(baseName) ofType:@(ext)];
}

std::string mkshot_fs::getPathForAsset(const char *baseName, const char *ext)
{
    NSString *assetPath = getPathForAsset_internal(baseName, ext);

    if (assetPath == nil)
        throw Exception(Exception::NoFileError, "Failed to find the asset named %s.%s", baseName, ext);

    return std::string(NSTOPATH(getPathForAsset_internal(baseName, ext)));
}

std::string mkshot_fs::contentsOfAssetAsString(const char *baseName, const char *ext)
{
    NSString *path = getPathForAsset_internal(baseName, ext);
    NSString *fileContents = [NSString stringWithContentsOfFile:path encoding:NSUTF8StringEncoding error:NULL];

    if (fileContents == nil)
        throw Exception(Exception::MKShotError, "Failed to read file at %s", path.UTF8String);

    return std::string(fileContents.UTF8String);
}

std::string mkshot_fs::getResourcePath()
{
    return std::string(NSTOPATH(NSBundle.mainBundle.resourcePath));
}

std::string mkshot_fs::selectPath(SDL_Window *win, const char *msg, const char *prompt)
{
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    panel.canChooseDirectories = true;
    panel.canChooseFiles = false;

    if (msg)
        panel.message = @(msg);

    if (prompt)
        panel.prompt = @(prompt);

    //panel.directoryURL = [NSURL fileURLWithPath:NSFileManager.defaultManager.currentDirectoryPath];

    SDL_SysWMinfo wm {};
    SDL_GetWindowWMInfo(win, &wm);

    [panel beginSheetModalForWindow:wm.info.cocoa.window completionHandler:^(NSModalResponse res) {
        [NSApp stopModalWithCode:res];
    }];

    [NSApp runModalForWindow:wm.info.cocoa.window];

    // The window needs to be brought to the front again after the OpenPanel closes
    [wm.info.cocoa.window makeKeyAndOrderFront:nil];
    if (panel.URLs.count > 0)
        return std::string(NSTOPATH(panel.URLs[0].path));

    return std::string();
}
