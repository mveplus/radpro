/*
 * Rad Pro
 * Simulator display
 *
 * (C) 2022-2024 Gissio
 *
 * License: MIT
 */

#if defined(SIMULATOR)

#include <SDL.h>

#include <mcu-renderer-sdl.h>

#include "../cstring.h"
#include "../display.h"
#include "../settings.h"
#include "../system.h"

// Controller

extern mr_t mr;

bool displayOn;
bool displayBacklightOn;
bool pulseLEDOn;
bool alertLEDOn;
bool vibratorOn;

static uint8_t displayBrightnessValues[] = {
    0x3f, 0x7f, 0xbf, 0xff};

extern float tubeCPS;

void initDisplayController(void)
{
    // mcu-renderer
#if defined(DISPLAY_MONOCHROME)
    mr_sdl_init(&mr,
                DISPLAY_WIDTH,
                DISPLAY_HEIGHT,
                MR_SDL_DISPLAY_TYPE_MONOCHROME,
                DISPLAY_UPSCALE,
                FIRMWARE_NAME);
#elif defined(DISPLAY_COLOR)
    mr_sdl_init(&mr,
                DISPLAY_WIDTH,
                DISPLAY_HEIGHT,
                MR_SDL_DISPLAY_TYPE_COLOR,
                DISPLAY_UPSCALE,
                FIRMWARE_NAME);
#endif
}

void setDisplayOn(bool value)
{
    displayOn = value;

    mr_sdl_set_display(&mr, displayOn);
}

bool isDisplayOn(void)
{
    return displayOn;
}

void updateDisplayContrast(void)
{
}

void refreshDisplay(void)
{
}

void updateDisplayTitle(void);

void updateDisplay(void)
{
    mr_sdl_refresh_display(&mr);

    SDL_Event event;
    if (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            writeDatalog();
            writeSettings();

            exit(0);

            break;

        case SDL_KEYDOWN:
        {
            float tubeCPSAdjustment = expf(logf(10) / 20);

            if (event.key.keysym.mod & KMOD_LCTRL)
                tubeCPS /= tubeCPSAdjustment;
            else if (event.key.keysym.mod & KMOD_RCTRL)
                tubeCPS *= tubeCPSAdjustment;

            if (tubeCPS < 0.01F)
                tubeCPS = 0.01F;
            else if (tubeCPS > 100000.0F)
                tubeCPS = 100000.0F;

            updateDisplayTitle();

            break;
        }
        }
    }
}

// const Uint8 *state = SDL_GetKeyboardState(NULL);

// if (state[SDL_SCANCODE_LCTRL])
//     tubeCPS /= 1.001F;
// else if (state[SDL_SCANCODE_RCTRL])
//     tubeCPS *= 1.001F;

void updateDisplayTitle(void)
{
    char buffer[256];

    sprintf(buffer, "%s (%.2f cps)", FIRMWARE_NAME, tubeCPS);

    if (pulseLEDOn || vibratorOn)
        strcat(buffer, " ");
    if (pulseLEDOn)
        strcat(buffer, "🔴");
    if (alertLEDOn)
        strcat(buffer, "⚠️");
    if (vibratorOn)
        strcat(buffer, "📳");

    mr_sdl_set_title(&mr, buffer);
}

// Backlight

void initDisplayBacklight(void)
{
}

void setDisplayBacklightOn(bool value)
{
    displayBacklightOn = value;

    mr_sdl_set_backlight(
        &mr,
        value
            ? displayBrightnessValues[settings.displayBrightness]
            : 0);
}

bool isDisplayBacklightOn(void)
{
    return displayBacklightOn;
}

// LED

void initLEDController(void)
{
}

void setPulseLED(bool value)
{
    pulseLEDOn = value;

    updateDisplayTitle();
}

void setAlertLED(bool value)
{
    alertLEDOn = value;

    updateDisplayTitle();
}

// Vibrator

void initVibratorController(void)
{
}

void setVibrator(bool value)
{
    vibratorOn = value;

    updateDisplayTitle();
}

#endif
