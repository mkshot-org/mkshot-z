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

/* Fragment shader dealing with transitions */

uniform sampler2D currentScene;
uniform sampler2D frozenScene;
uniform sampler2D transMap;
/* Normalized */
uniform float prog;
/* Vague [0, 512] normalized */
uniform float vague;

varying vec2 v_texCoord;

void main()
{
    float transV = texture2D(transMap, v_texCoord).r;
    float cTransV = clamp(transV, prog, prog+vague);
    lowp float alpha = (cTransV - prog) / vague;
    
    vec4 newFrag = texture2D(currentScene, v_texCoord);
    vec4 oldFrag = texture2D(frozenScene, v_texCoord);
    
    gl_FragColor = mix(newFrag, oldFrag, alpha);
}
