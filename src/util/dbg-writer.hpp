/*
** debugwriter.h
**
** This file is part of mkxp, further modified for mkshot-z.
**
** Copyright (C) 2026 mkshot-z contributors <https://github.com/mkshot-org>
** Copyright (C) 2013 - 2021 Amaryllis Kulla <ancurio@mapleshrine.eu>
**
** mkxp is licensed under GPLv2 or later.
** mkshot-z is licensed under GPLv3 or later.
*/

#pragma once


#include <iostream>
#include <sstream>
#include <vector>

#ifdef __ANDROID__
#include <android/log.h>
#endif


/* A cheap replacement for qDebug() */

class Debug
{
public:
	Debug()
	{
		buf << std::boolalpha;
	}

	template<typename T>
	Debug &operator<<(const T &t)
	{
		buf << t;
		buf << " ";

		return *this;
	}

	template<typename T>
	Debug &operator<<(const std::vector<T> &v)
	{
		for (size_t i = 0; i < v.size(); ++i)
			buf << v[i] << " ";

		return *this;
	}

	~Debug()
	{
#ifdef __ANDROID__
		__android_log_write(ANDROID_LOG_DEBUG, "mkshot", buf.str().c_str());
#else
		std::cerr << buf.str() << std::endl;
#endif
	}

private:
	std::stringstream buf;
};


