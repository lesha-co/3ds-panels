#include <unistd.h>
#include <cstdio>
#include "3ds.h"
#include "FileManager.h"

using namespace std;
void drawASCII(){
    for (int i = 0; i < 256; ++i) {
        if( i%16 == 0) {
            printf("\n %3x |", i);
        }
        if(i == '\n' || i == '\r' || i == '\0' ){
            printf("X ");
        } else {
            printf("%-2c",i);
        }
    }
}
int main(int argc, char **argv)
{
    gfxInitDefault();
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// run once begin
    ///
    // init stuff
    chdir("/");
    FileManager fileManager("", "");

    ///
    /// run once end
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    while (aptMainLoop())
    {
        hidScanInput();
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// run loop begin
        ///
        u32 kHeld = 0;// hidKeysHeld();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) break; // break in order to return to hbmenu
        fileManager.clock(kDown, kHeld);
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
