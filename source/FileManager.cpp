//
// Created by lichevsky on 24.09.16.
//
#include "FileManager.h"

using namespace std;
int list_called = 0;

string getVerbByOpType(DiskOperationType mode){
    switch (mode){
        case COPY:
            return "Copy";
        case MOVE:
            return "Move";
        case DELETE:
            return "Delete";
    }
    return "";
}

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
                cout << getVerbByOpType(COPY) << "...";
                break;
            case MENU_DELETE:
                cout << getVerbByOpType(DELETE) << "...";
                break;
            case MENU_MOVE:
                cout << getVerbByOpType(MOVE) << "...";
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
        case MODE_PROMPT:
        {
            string verb = getVerbByOpType(pendingOperationType);
            preparePrompt(true);
            vector<FileInfo*> items = active->getMarkedItems(true);
            for (FileInfo* pItem : items){
                printf("Selected item: %s\n", pItem->name.c_str());
                string to_path = "";
                if(pendingOperationType != DELETE){
                    to_path = getInactivePane()->getCWD() + "/" + pItem->name;
                }
                queue.push_back(new DiskOperation(pendingOperationType, pItem->path, to_path));
            }
            if (queue.size() == 0) {
                printf("What.\n\n");
            } else {
                if (queue.size() == 1) {
                    cout << verb << " file?\n\n";
                } else {
                    cout << verb <<" [" << queue.size() << "] files?\n\n";
                }
                cout << "\tfrom: "<< active->getCWD() << "\n\n";
                if(pendingOperationType != DELETE){
                    cout << "\tto:   "<< getInactivePane()->getCWD() << "\n\n";
                }
                cout << "\t[A] OK\t[B] CANCEL\n";
            }
            break;
        }

        case MODE_NORMAL: {
            this->l->redraw();
            this->r->redraw();
            this->onCWDUpdate(active);
            break;
        }

        case MODE_OPERATION_PROGRESS:{
            //preparePrompt(true);
            if(this->queue.size() != 0){
                printf("\n\nCommencing operation\n");
                printf("%s\n", this->queue.front()->getSource().c_str());
                this->queue.front()->commence();
            } else{
                printf("\n\nFinished. [B] to close window");
                this->queue.clear();
                //setmode(MODE_NORMAL);
            }
            break;
        }
    }
}

void FileManager::clock_MODE_MENU(u32 kDown, u32 kHeld){
    if ((kHeld | kDown) & KEY_A){
        switch(menuConfig[selectedMenuItem]){
            case MENU_COPY:
                pendingOperationType = COPY;
                break;
            case MENU_DELETE:
                pendingOperationType = DELETE;
                break;
            case MENU_MOVE:
                pendingOperationType = MOVE;
                break;
        }
        setmode(MODE_PROMPT);
        return;
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
}
void FileManager::clock(u32 kDown, u32 kHeld){
    switch(this->mode){
        case MODE_MENU:{
            clock_MODE_MENU(kDown, kHeld);
            break;
        }
        case MODE_PROMPT: {
            if (kDown & KEY_A){
                this->setmode(MODE_OPERATION_PROGRESS);
            }
            if (kDown & KEY_B){
                this->queue.clear();
                this->setmode(MODE_NORMAL);
            }
            break;
        }
        case MODE_NORMAL: {
            u32 pane_controls_mask = (KEY_DOWN | KEY_UP | KEY_DLEFT | KEY_DRIGHT | KEY_A | KEY_B | KEY_X);
            if ((kHeld | kDown) & pane_controls_mask) {
                active->clock(kDown & pane_controls_mask,
                              kHeld & pane_controls_mask);
            }
            if (kDown & KEY_Y) {
                this->setmode(MODE_MENU);
            }
            if (kDown & KEY_L) {
                active = l;
                l->setActive(true);
                r->setActive(false);
                this->onCWDUpdate(active);
            }
            if (kDown & KEY_R) {
                active = r;
                r->setActive(true);
                l->setActive(false);
                this->onCWDUpdate(active);
            }
            break;
        }
        case MODE_OPERATION_PROGRESS: {
            if (kDown & KEY_B){
                this->setmode(MODE_NORMAL);
            }
            if(this->queue.size() != 0){
                this->queue.front()->tick();
                double progress = this->queue.front()->get_progress();
                //u32 pc = (u32)(progress*100);
                u32 current_line = (u32)prompt_body.cursorY;
                //cout << "\t" << pc <<"% completed\n";
                cout << position(current_line, 0);
                cout << draw_progressbar((u32)prompt_body.windowWidth, progress);
                if (this->queue.front()->is_finished()) {
                    this->queue.erase(this->queue.begin());
                    // resetting mode_operation_progress so it grabs next job from queue;
                    setmode(MODE_OPERATION_PROGRESS);
                }
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
        up.marked = false;
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
            fi.marked = false;
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