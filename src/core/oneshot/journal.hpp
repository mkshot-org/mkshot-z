/*
** journal.hpp
**
** This file is part of ModShot-mkxp-z, further modified for mkshot-z.
**
** Copyright (C) 2026 mkshot-z contributors <https://github.com/mkshot-org>
** Copyright (C) 2024 hat_kid <https://github.com/thehatkid>
**
** ModShot-mkxp-z is licensed under GPLv2 or later.
** mkshot-z is licensed under GPLv3-or-later.
*/

#pragma once

struct JournalPrivate;

class Journal
{
private:
	JournalPrivate *p;

public:
	Journal();
	~Journal();

	bool isActive() const;
	void set(const char *name);
	void setLang(const char *lang);

	void nikoPrepare();
	void nikoStart();
};
