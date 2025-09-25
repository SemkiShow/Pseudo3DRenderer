#include "Game.hpp"
#include "Render.hpp"
#include "Settings.hpp"
#include <raygui.h>
#include <raylib.h>

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

int main()
{
    Load();

    int flags = 0;
    if (vsync) flags |= FLAG_VSYNC_HINT;
#if !defined(PLATFORM_WEB)
    flags |= FLAG_WINDOW_HIGHDPI;
    flags |= FLAG_WINDOW_RESIZABLE;
#endif
    SetConfigFlags(flags);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 24);

    InitWindow(windowSize.x, windowSize.y, "Pseudo3DRenderer");
    SetExitKey(-1);

    GuiSetFont(GetFontDefault());

    while (!WindowShouldClose())
    {
        DrawFrame();
    }

    Save();
    CloseWindow();

    return 0;
}
