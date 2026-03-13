/*
** This file is part of mkxp-z, further modified for mkshot-z.
**
** Copyright (C) 2026 mkshot-z contributors <https://github.com/mkshot-org>
** Created by ゾロアーク on 12/29/20.
**
** mkxp-z is licensed under GPLv2 or later.
** mkshot-z is licensed under GPLv3 or later.
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
