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

uniform sampler2D texture;
uniform mediump float hueAdjust;

varying vec2 v_texCoord;

/* Source: gamedev.stackexchange.com/a/59808/24839 */
vec3 rgb2hsv(vec3 c)
{
	const vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
	vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
	vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

	float d = q.x - min(q.w, q.y);

	/* Avoid divide-by-zero situations by adding a very tiny delta.
	 * Since we always deal with underlying 8-Bit color values, this
	 * should never mask a real value */
	const float eps = 1.0e-10;

	return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + eps)), d / (q.x + eps), q.x);
}

vec3 hsv2rgb(vec3 c)
{
	const vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main ()
{
	vec4 color = texture2D (texture, v_texCoord.xy);
	vec3 hsv = rgb2hsv(color.rgb);

	hsv.x += hueAdjust;
	color.rgb = hsv2rgb(hsv);

	gl_FragColor = color;
}
