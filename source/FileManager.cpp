//
// Created by lichevsky on 24.09.16.
//
#include "FileManager.h"

using namespace std;
int list_called = 0;
FileManager::FileManager(string cwd_left, string cwd_right){
    setupConsoles();
    this->l = new FilePane(leftpan, this);
    this->r = new FilePane(rightpan, this);
    this->l->setContext(cwd_left);
    this->r->setContext(cwd_right);
    this->active = l;
    this->mode = MODE_NORMAL;
    l->setActive(true);
    this->onCWDUpdate(active);
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
    consoleInit(GFX_TOP, &prompt);
    consoleInit(GFX_TOP, &prompt_body);
    //                                x   y   w   h
    consoleSetWindow(&menu,           0,  0, 50,  1);
    consoleSetWindow(&leftpan,        0,  1, 25, 26);
    consoleSetWindow(&rightpan,      25,  1, 25, 26);
    consoleSetWindow(&under_panels,   0, 27, 50,  2);
    consoleSetWindow(&actions,        0, 29, 50,  1);

    u8 prompt_horizontal_margin = 5;
    u8 prompt_vertical_margin = 2;
    u8 prompt_horizontal_padding = 2;
    u8 prompt_vertical_padding = 2;
    consoleSetWindow(&prompt,
                     prompt_horizontal_margin,
                     prompt_vertical_margin,
                     50 - prompt_horizontal_margin*2,
                     30 - prompt_vertical_margin*2);
    consoleSetWindow(&prompt_body,
                     prompt_horizontal_margin+prompt_horizontal_padding,
                     prompt_vertical_margin+prompt_vertical_padding,
                     50 - (prompt_horizontal_margin + prompt_horizontal_padding)*2,
                     30 - (prompt_vertical_margin + prompt_vertical_padding)*2);


    consoleSelect(&bottom);
    cout << BG_DEFAULT;
    consoleClear();
    consoleSelect(&menu);
    cout << BG_DEFAULT;
    consoleClear();
    cout << "menu";

    consoleSelect(&actions);
    cout << BG_DEFAULT;
    consoleClear();
    cout << "actions";

}

PrintConsole* FileManager::getBottomConsole(){
    return &bottom;
}
void FileManager::DeleteFile(string path){
    remove(path.c_str());
    active->redraw();
}
void FileManager::setmode(DisplayMode_t mode){
    this->mode = mode;
    switch(this->mode){
        case MODE_PROMPT_DELETE: {
            consoleSelect(&prompt);
            cout << BG_PROMPT;
            consoleClear();
            drawBorder(border_double, (u32)prompt.windowWidth, (u32)prompt.windowHeight);
            consoleSelect(&prompt_body);
            cout << BG_PROMPT;
            consoleClear();
            string name = active->getSelectedItem().name;
            printf("Delete file?\n\n\t%s\n\n\t[A] OK\t[B] CANCEL ", name.c_str());
            break;
        }
        case MODE_NORMAL: {
            this->l->redraw();
            this->r->redraw();
            this->onCWDUpdate(active);
            break;
        }
        default:
            break;
    }
}
void FileManager::clock(u32 kDown, u32 kHeld){
    switch(this->mode){
        case MODE_PROMPT_DELETE:{
            if (kDown & KEY_A){
                FileInfo f = active->getSelectedItem();
                DeleteFile(f.path);
                this->setmode(MODE_NORMAL);
            }
            if (kDown & KEY_B){
                this->setmode(MODE_NORMAL);
            }
            break;
        }
        case MODE_NORMAL:
        default:

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
            if (kDown & KEY_X){
                this->setmode(MODE_PROMPT_DELETE);
            }
            if (kDown & KEY_L){
                active = l;
                l->setActive(true);
                r->setActive(false);
                this->onCWDUpdate(active);
            }
            if (kDown & KEY_R){
                active = r;
                r->setActive(true);
                l->setActive(false);
                this->onCWDUpdate(active);
            }

            break;
    }

}


vector<FileInfo> FileManager::list_files(string dir){
    list_called++;
    consoleSelect(&this->actions);
    printf("\n%d",list_called);
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
void FileManager::onCWDUpdate(FilePane* pane){
    if (pane == this->active){
        consoleSelect(&this->under_panels);
        cout << BG_DEFAULT;
        consoleClear();
        string cwd = pane->getCWD();
        if(cwd == "")
            cwd = "/";
        cout << cwd;
    }
}