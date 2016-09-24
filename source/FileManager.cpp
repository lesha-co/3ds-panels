//
// Created by lichevsky on 24.09.16.
//
#include <unistd.h>
#include "FileManager.h"

FileManager::FileManager(string cwd_left, string cwd_right){
    setupConsoles();
    this->l = new FilePane(leftpan, this);
    this->r = new FilePane(rightpan, this);
    this->l->setCWD(cwd_left);
    this->r->setCWD(cwd_right);
    this->active = l;
    l->setActive(true);
}

FileManager::~FileManager(){
    free(this->l);
    free(this->r);
}

void FileManager::setupConsoles(){
    consoleInit(GFX_BOTTOM, &bottom);
    consoleInit(GFX_TOP, &menu);
    consoleInit(GFX_TOP, &leftpan);
    consoleInit(GFX_TOP, &rightpan);
    consoleInit(GFX_TOP, &under_panels);
    consoleInit(GFX_TOP, &actions);
    consoleInit(GFX_TOP, &context);
    //                                x   y   w   h
    consoleSetWindow(&menu,           0,  0, 50,  1);
    consoleSetWindow(&leftpan,        0,  1, 25, 26);
    consoleSetWindow(&rightpan,      25,  1, 25, 26);
    consoleSetWindow(&under_panels,   0, 27, 50,  2);
    consoleSetWindow(&actions,        0, 29, 50,  1);
    consoleSetWindow(&context,        5,  1, 20, 20);
}

PrintConsole* FileManager::getBottomConsole(){
    return &bottom;
}

void FileManager::clock(u32 kDown, u32 kHeld){
    if ((kHeld | kDown) & KEY_DOWN){
        active->moveDown();
    }
    if ((kHeld | kDown) & KEY_UP){
        active->moveUp();
    }
    if ((kHeld | kDown) & KEY_DLEFT){
        active->moveTop();
    }
    if ((kHeld | kDown) & KEY_DRIGHT){
        active->moveEnd();
    }
    if (kDown & KEY_A){
        active->enter();
    }
    if (kDown & KEY_B){
        active->updir();
    }
    if (kDown & KEY_L){
        active = l;
        l->setActive(true);
        r->setActive(false);
    }
    if (kDown & KEY_R){
        active = r;
        r->setActive(true);
        l->setActive(false);
    }
}


vector<FileInfo> FileManager::list_files(string dir){
    consoleSelect(&this->bottom);
    printf("Listing directory %s", dir.c_str());
    vector<FileInfo> v;
    if (dir != ""){
        FileInfo up;
        up.name = UPDIR;
        up.special = true;
        v.push_back(up);
    }
    DIR *dp;
    dirent *dirp;

    dp = opendir(dir.c_str() );
    if (dp == NULL)
    {
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