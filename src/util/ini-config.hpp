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

#include <iostream>
#include <map>

class INIConfiguration
{
	class Section
	{
		friend class INIConfiguration;

		struct Property
		{
			std::string m_Name;
			std::string m_Value;
		};

		typedef std::map<std::string, Property> property_map;
	public:
		Section (const Section& s) = default;
		Section (Section&& s) = default;

		bool getStringProperty (const std::string& name, std::string& outPropStr) const;

	private:
		explicit Section (const std::string& name);

		std::string m_Name;
		property_map m_PropertyMap;
	};

	typedef std::map<std::string, Section> section_map;
public:
	bool load (std::istream& inStream);

	std::string getStringProperty(const std::string& sname, const std::string& name, const std::string& def = "") const;

protected:
	void addProperty (const std::string& sname, const std::string& name, const std::string& val);

private:
	section_map m_SectionMap;
};


