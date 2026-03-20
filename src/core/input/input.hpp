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
#include <SDL3/SDL_gamepad.h>
#include <string>
#include <vector>

extern std::unordered_map<int, int> vKeyToScancode;
extern std::unordered_map<std::string, int> strToScancode;
extern std::unordered_map<std::string, SDL_GamepadButton> strToGCButton;

struct InputPrivate;
struct RGSSThreadData;

class Input
{
public:
	enum ButtonCode
	{
		None = 0,

		Down = 2, Left = 4, Right = 6, Up = 8,

		Action = 11, Cancel = 12, Menu = 13,
		Items = 14, Run = 15, Deactivate = 16,

		L = 17, R = 18,

		Shift = 21, Ctrl = 22, Alt = 23,

		F5 = 25, F6 = 26, F7 = 27, F8 = 28, F9 = 29,

		/* Non-standard extensions */
		MouseLeft = 38, MouseMiddle = 39, MouseRight = 40,
		MouseX1 = 41, MouseX2 = 42
	};

    void recalcRepeat(unsigned int fps);

    double getDelta();
	void update();

    std::vector<std::string> getBindings(ButtonCode code);

	bool isPressed(int button);
	bool isTriggered(int button);
	bool isRepeated(int button);
    bool isReleased(int button);
    unsigned int count(int button);
    double repeatTime(int button);

    bool isPressedEx(int code, bool isVKey);
    bool isTriggeredEx(int code, bool isVKey);
    bool isRepeatedEx(int code, bool isVKey);
    bool isReleasedEx(int code, bool isVKey);
    unsigned int repeatcount(int code, bool isVKey);
    double repeatTimeEx(int code, bool isVKey);

    bool controllerIsPressedEx(int button);
    bool controllerIsTriggeredEx(int button);
    bool controllerIsRepeatedEx(int button);
    bool controllerIsReleasedEx(int button);
    unsigned int controllerRepeatcount(int button);
    double controllerRepeatTimeEx(int button);

    uint8_t *rawKeyStates();
    unsigned int rawKeyStatesLength();
    uint8_t *rawButtonStates();
    unsigned int rawButtonStatesLength();
    int16_t *rawAxes();
    unsigned int rawAxesLength();

    short getControllerAxisValue(SDL_GamepadAxis axis);

	int dir4Value();
	int dir8Value();

	int mouseX();
	int mouseY();
    int scrollV();
    bool mouseInWindow();

    bool getControllerConnected();
    const char *getControllerName();
    int getControllerPowerLevel();

    bool getTextInputMode();
    void setTextInputMode(bool mode);
    const char *getText();
    void clearText();

    char *getClipboardText();
    void setClipboardText(char *text);

    const char *getAxisName(SDL_GamepadAxis axis);
    const char *getButtonName(SDL_GamepadButton button);

	bool hasQuit();

private:
	Input(const RGSSThreadData &rtData);
	~Input();

	friend struct SharedStatePrivate;

	InputPrivate *p;
};


