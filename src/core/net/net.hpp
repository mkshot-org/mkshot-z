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

#pragma once

#include <unordered_map>
#include <string>

namespace mkshot_net {

typedef std::unordered_map<std::string, std::string> StringMap;

class HTTPResponse {
public:
    int status();
    std::string &body();
    StringMap &headers();
    ~HTTPResponse();

private:
    int _status;
    std::string _body;
    StringMap _headers;
    HTTPResponse();

    friend class HTTPRequest;
};

class HTTPRequest {
public:
    HTTPRequest(const char *dest, bool follow_redirects = true);
    ~HTTPRequest();

    StringMap &headers();

    std::string destination;

    HTTPResponse get();
    HTTPResponse post(StringMap &postData);
    HTTPResponse post(const char *body, const char *content_type);
private:
    StringMap _headers;
    bool follow_location;
};
}
