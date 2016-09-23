#include <3ds.h>
#include <cstdio>
#include <iostream>
#include <dirent.h>
#include "../3ds_string_utils/source/string_utils.h"
#include "FilePane.h"

using namespace std;

void a(vector<int>* c){
    c->push_back(1);

}
int main(int argc, char **argv)
{
    gfxInitDefault();
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// run once begin
    ///
    consoleInit(GFX_TOP, NULL);
    PrintConsole menu, leftpan, rightpan, console, actions, context, all;
    consoleInit(GFX_TOP, &leftpan);
    consoleInit(GFX_TOP, &rightpan);
    consoleInit(GFX_TOP, &console);
    consoleInit(GFX_TOP, &actions);
    consoleInit(GFX_TOP, &context);
    consoleInit(GFX_TOP, &all);

                    //           x   y  w   h
    consoleSetWindow(&menu,      0,  0, 50, 1);
    consoleSetWindow(&leftpan,   0,  1, 25, 26);
    consoleSetWindow(&rightpan, 25,  1, 25, 26);
    consoleSetWindow(&console,   0, 27, 50, 2);
    consoleSetWindow(&actions,   0, 29, 50, 1);
    consoleSetWindow(&context,   5, 1, 20, 20);

    FilePane leftFilePane(leftpan, "");
    FilePane rightFilePane(rightpan, "");
    FilePane* active;
    active = &leftFilePane;
    leftFilePane.setActive(true);


    ///
    /// run once end
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    while (aptMainLoop())
    {
        hidScanInput();
        u32 kHeld = hidKeysHeld();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) break; // break in order to return to hbmenu
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// run loop begin
        ///
        if ((kHeld | kDown) & KEY_DOWN){
            active->moveDown();
        }
        if ((kHeld | kDown) & KEY_UP){
            active->moveUp();
        }
        if (kDown & KEY_A){
            active->enter();
        }
        if (kDown & KEY_DLEFT){
            active = &leftFilePane;
            leftFilePane.setActive(true);
            rightFilePane.setActive(false);
        }
        if (kDown & KEY_DRIGHT){
            active = &rightFilePane;
            rightFilePane.setActive(true);
            leftFilePane.setActive(false);
        }

        ///
        /// run loop end
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Flush and swap framebuffers
        gfxFlushBuffers();
        gfxSwapBuffers();

        //Wait for VBlank
        gspWaitForVBlank();
    }

    // Exit services
    gfxExit();

    return 0;
}
