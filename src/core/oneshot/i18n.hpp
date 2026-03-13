/*
** This file is part of ModShot-mkxp-z, further modified for mkshot-z.
**
** Copyright (C) 2026 mkshot-z contributors <https://github.com/mkshot-org>
** Copyright (C) 2024 hat_kid <https://github.com/thehatkid>
**
** ModShot-mkxp-z is licensed under GPLv2 or later.
** mkshot-z is licensed under GPLv3-or-later.
*/

#ifndef ONESHOT_I18N
#define ONESHOT_I18N

#include <string>

namespace OneshotImpl
{
	namespace i18n
	{
		void loadLocale(const char *locale);
		void unloadLocale();

		void loadLanguageMetadata();
		void unloadLanguageMetadata();

		std::string findtext(const char *message);

		std::string getFontName();
		int getFontSize();

		std::string stringUnescape(const std::string &input);
	}
}

#endif // ONESHOT_I18N
