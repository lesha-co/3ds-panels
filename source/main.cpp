#include <3ds.h>
#include <cstdio>
#include <iostream>

#include <fstream>

#include <dirent.h>
#include "../3ds_string_utils/source/string_utils.h"
#include "FilePane.h"

using namespace std;

vector<FileInfo> readdir(string dir){
    vector<FileInfo> v;
    DIR *dp;
    struct dirent *dirp;
    string pref = "/";

    dp = opendir( (pref +dir).c_str() );
    if (dp == NULL)
    {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return v;

    } else {
        while ((dirp = readdir( dp )))
        {
            FileInfo fi;
            fi.name = dirp->d_name;
            fi.path = dir + "/" + dirp->d_name;

            stat( fi.path.c_str(), &fi.stats );

            v.push_back(fi);
        }
        closedir( dp );
    }
    return v;
}

int main(int argc, char **argv)
{
    gfxInitDefault();
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /// run once begin
    ///
    consoleInit(GFX_TOP, NULL);
    /*
    PrintConsole menu, leftpan, rightpan, console, actions, context;

    consoleInit(GFX_TOP, &leftpan);
    consoleInit(GFX_TOP, &rightpan);
    consoleInit(GFX_TOP, &console);
    consoleInit(GFX_TOP, &actions);
    consoleInit(GFX_TOP, &context);

                    //           x   y  w   h
    consoleSetWindow(&menu,      0,  0, 50, 1);
    consoleSetWindow(&leftpan,   0,  1, 25, 26);
    consoleSetWindow(&rightpan, 25,  1, 25, 26);
    consoleSetWindow(&console,   0, 27, 50, 2);
    consoleSetWindow(&actions,   0, 29, 50, 1);
    consoleSetWindow(&context,   5, 1, 20, 20);
    */
    vector<FileInfo> f = readdir("");
    vector<FileInfo> f1 = readdir("!a9lh");
    printf("DCIM/100NIN03 %d\n", f.size());
    for (FileInfo fi:f) {
        printf("%s %s\n", S_ISDIR(fi.stats.st_mode)?"DIR":"FIL", fi.name.c_str());
    }
    printf("a9lh %d\n", f1.size());

    for (FileInfo fi:f1) {
        printf("%s %s\n", S_ISDIR(fi.stats.st_mode)?"DIR":"FIL", fi.name.c_str());
    }






    ///
    /// run once end
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    while (aptMainLoop())
    {
        hidScanInput();
        u32 kDown = hidKeysDown();
        if (kDown & KEY_START) break; // break in order to return to hbmenu
        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// run loop begin
        ///

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
