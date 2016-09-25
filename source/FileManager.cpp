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
    menuConfig = {MENU_COPY, MENU_MOVE, MENU_DELETE};
    selectedMenuItem = 0;
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
    //active->redraw();
}
void FileManager::CopyFile(string from, string to){
    const char* from_path = from.c_str();
    const char* to_path = to.c_str();
    printf("\n\n\t%s", from_path);
    printf("\n\n\t%s", to_path);

    char buf[BUFSIZ];
    size_t size;

    FILE* source = fopen(from_path, "rb");
    FILE* dest = fopen(to_path, "wb");


    printf("\n\n\tstarting...");
    while (true) {
        size = fread(buf, 1, BUFSIZ, source);
        if (size <= 0)
            break;
        fwrite(buf, 1, size, dest);
    }
    fclose(source);
    fclose(dest);

    printf("\n\n\tOK");

}

void FileManager::preparePrompt(bool selectBody){
    consoleSelect(&prompt);
    cout << BG_PROMPT;
    consoleClear();
    drawBorder(border_double, &prompt);
    if(selectBody){
        consoleSelect(&prompt_body);
        cout << BG_PROMPT;
        consoleClear();
    }
}
void FileManager::drawMenu(){
    preparePrompt(true);
    for (u32 i = 0; i < menuConfig.size(); ++i) {
        if(i == selectedMenuItem){
            cout << BG_PROMPT_HIGHLIGHT;
        }
        cout << position(i,0);
        switch(menuConfig[i]){
            case MENU_COPY:
                cout << "Copy...";
                break;
            case MENU_DELETE:
                cout << "Delete...";
                break;
            case MENU_MOVE:
                cout << "Move...";
                break;
        }
        if(i == selectedMenuItem){
            cout << BG_PROMPT;
        }
    }
}
void FileManager::setmode(DisplayMode_t mode){
    this->mode = mode;
    switch(this->mode){
        case MODE_MENU:{
            drawMenu();
            break;
        }
        case MODE_PROMPT_DELETE: {
            preparePrompt(true);
            string name = active->getSelectedItem().name;
            printf("Delete file?\n\n\t%s\n\n\t[A] OK\t[B] CANCEL ", name.c_str());
            break;
        }
        case MODE_PROMPT_COPY: {
            preparePrompt(true);
            string name = active->getSelectedItem().name;
            string to_path = getInactivePane()->getCWD()+"/"+name;
            printf("Copy file\n\n\t%s\n\n\tfrom: %s\n\n\tto:   %s\n\n\t[A] OK\t[B] CANCEL ",
                   name.c_str(),
                   active->getCWD().c_str(),
                   to_path.c_str()
            );
            break;
        }
        case MODE_NORMAL: {
            this->l->redraw();
            this->r->redraw();
            this->onCWDUpdate(active);
            break;
        }

        case MODE_PROMPT_MOVE:{
            preparePrompt(true);
            string name = active->getSelectedItem().name;
            string to_path = getInactivePane()->getCWD()+"/"+name;
            printf("Move file\n\n\t%s\n\n\tfrom: %s\n\n\tto:   %s\n\n\t[A] OK\t[B] CANCEL ",
                   name.c_str(),
                   active->getCWD().c_str(),
                   to_path.c_str()
            );
            break;
        }
    }
}
void FileManager::clock(u32 kDown, u32 kHeld){
    switch(this->mode){
        case MODE_MENU:{
            if ((kHeld | kDown) & KEY_A){
                switch(menuConfig[selectedMenuItem]){
                    case MENU_COPY:
                        setmode(MODE_PROMPT_COPY);
                        break;
                    case MENU_DELETE:
                        setmode(MODE_PROMPT_DELETE);
                        break;
                    case MENU_MOVE:
                        setmode(MODE_PROMPT_MOVE);
                        break;
                }
            }
            if ((kHeld | kDown) & KEY_DOWN){
                if (selectedMenuItem >= menuConfig.size() -1){
                    selectedMenuItem = 0;
                } else {
                    selectedMenuItem ++;
                }
                drawMenu();
            }
            if ((kHeld | kDown) & KEY_LEFT){
                selectedMenuItem = 0;
                drawMenu();
            }
            if ((kHeld | kDown) & KEY_RIGHT){
                selectedMenuItem = menuConfig.size() -1;
                drawMenu();
            }
            if ((kHeld | kDown) & KEY_UP){
                if (selectedMenuItem == 0){
                    selectedMenuItem = menuConfig.size() -1;
                } else {
                    selectedMenuItem --;
                }
                drawMenu();
            }
            if ((kHeld | kDown) & KEY_B){
                this->setmode(MODE_NORMAL);
            }
            break;
        }

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
        case MODE_PROMPT_COPY:{
            if (kDown & KEY_A){
                FileInfo f = active->getSelectedItem();
                string to_path = getInactivePane()->getCWD()+"/"+f.name;
                CopyFile(f.path, to_path);
                this->setmode(MODE_NORMAL);
            }
            if (kDown & KEY_B){
                this->setmode(MODE_NORMAL);
            }
            break;
        }
        case MODE_NORMAL:

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
            if (kDown & KEY_Y){
                this->setmode(MODE_MENU);
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
        case MODE_PROMPT_MOVE:{
            if (kDown & KEY_A){
                FileInfo f = active->getSelectedItem();
                string to_path = getInactivePane()->getCWD()+"/"+f.name;
                CopyFile(f.path, to_path);
                DeleteFile(f.path);
                this->setmode(MODE_NORMAL);
            }
            if (kDown & KEY_B){
                this->setmode(MODE_NORMAL);
            }
            break;
        }
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

FilePane* FileManager::getInactivePane(){
    return (active == l)? r : l;
}