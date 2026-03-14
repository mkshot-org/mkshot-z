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

uniform mat4 projMat;

uniform vec2 texSizeInv;
uniform vec2 translation;

uniform vec2 aniOffset;

attribute vec2 position;
attribute vec2 texCoord;

varying vec2 v_texCoord;

const vec2 atAreaA = vec2(9.0*32.0, 12.0*32.0);
const float atAreaCX = 12.0*32.0;
const float atAreaCW = 4.0*32.0;

void main()
{
	vec2 tex = texCoord;
	lowp float pred;

	/* Type A autotiles shift horizontally */
	pred = float(tex.x <= atAreaA.x && tex.y <= atAreaA.y);
	tex.x += aniOffset.x * pred;

	/* Type C autotiles shift vertically */
	pred = float(tex.x >= atAreaCX && tex.x <= (atAreaCX+atAreaCW) && tex.y <= atAreaA.y);
	tex.y += aniOffset.y * pred;

	gl_Position = projMat * vec4(position + translation, 0, 1);

	v_texCoord = tex * texSizeInv;
}
