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

uniform highp int aniIndex;

attribute vec2 position;
attribute vec2 texCoord;

varying vec2 v_texCoord;

const int nAutotiles = 7;
const float tileW = 32.0;
const float tileH = 32.0;
const float autotileW = 3.0*tileW;
const float autotileH = 4.0*tileW;
const float atAreaW = autotileW;
const float atAreaH = autotileH*float(nAutotiles);
const float atAniOffsetX = 3.0*tileW;
const float atAniOffsetY = tileH;

uniform lowp int atFrames[nAutotiles];

void main()
{
    vec2 tex = texCoord;
    lowp int atIndex = int(tex.y / autotileH);

    lowp int pred = int(tex.x <= atAreaW && tex.y <= atAreaH);
    lowp int frame = int(aniIndex - atFrames[atIndex] * (aniIndex / atFrames[atIndex]));
    lowp int row = frame / 8;
    lowp int col = frame - 8 * row;
    tex.x += atAniOffsetX * float(col * pred);
    tex.y += atAniOffsetY * float(row * pred);

    gl_Position = projMat * vec4(position + translation, 0, 1);

    v_texCoord = tex * texSizeInv;
}
