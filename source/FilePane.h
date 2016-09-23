//
// Created by lichevsky on 20.09.16.
//

#ifndef MAIN_EXEC_FILEPANE_H
#define MAIN_EXEC_FILEPANE_H

#include <fstream>
#include <3ds.h>
#include <iostream>
#include <dirent.h>
#include <string>
#include <vector>
#include <sys/stat.h>
#include <sstream>
#include "common.h"
#define  UPDIR ".."
const char VERTICAL_BORDER = (char)0xB3;
const char HORIZONTAL_BORDER = (char)0xC4;
const char TOP_LEFT_CORNER_BORDER = (char)0xDA;
const char TOP_RIGHT_CORNER_BORDER = (char)0xBF;
const char BOTTOM_LEFT_CORNER_BORDER = (char)0xC0;
const char BOTTOM_RIGHT_CORNER_BORDER = (char)0xD9;
const string BG_DEFAULT = setColor(COLOR_WHITE, COLOR_BLUE);
const string BG_HIGHLIGHT = setColor(COLOR_BLACK, COLOR_CYAN);
template <class T>
inline string to_string (const T& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}
inline string repeat(string in, u32 times){
    string s = "";
    for (u32 i = 0; i < times; ++i) {
        s += in;
    }
    return s;
}
using namespace std;
typedef struct dirent dirent;
typedef struct stat s_stat;
const u8 FILENAME_WIDTH = 16;
struct FileInfo{
    string name;
    string path;
    bool special;
    s_stat stats;
};
struct DisplayContext{
    string cwd;
    u32 selectedItem = 0;
    u32 startingIndex = 0;
};
vector<FileInfo> list_files(string dir){
    vector<FileInfo> v;
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


class FilePane {
    // special case:
    // index 0 represents UP-DIR (..)
    // index X represents items[X-1]
public:
    FilePane(PrintConsole printConsole, string cwd) {
        this->printConsole = printConsole;
        this->active = false;
        this->updir.name=UPDIR;
        this->updir.special = true;
        this->setCWD(cwd);
        this->draw();
    }

    void setActive(bool active){
        this->active = active;
        draw();
    }


    FileInfo getItem(u32 index) {
        if(index == 0){
            return updir;
        } else {
            return this->items[index-1];
        }
    }

    FileInfo getSelectedItem() {
        return getItem(this->ctx.selectedItem);
    }

    string getCWD() {
        return this->ctx.cwd;
    }

    void setCWD(string cwd) {
        this->ctx.cwd = cwd;
        this->items = list_files(cwd);
        this->ctx.selectedItem = 0;
        this->ctx.startingIndex = 0;
    }

    void moveUp(){
        if(this->ctx.selectedItem > 0){
            this->ctx.selectedItem--;
            // если курсор совпадает с самым верхним отображающимся элементом
            if (this->ctx.selectedItem == this->ctx.startingIndex){
                // если после перемещения курсор не стал совпадать с самым первым элементом
                if (this->ctx.selectedItem > 0){
                    //двигаем окно вверх
                    this->ctx.startingIndex = this->ctx.selectedItem-1;
                }
            }
            draw();
        }
    }

    void moveDown(){
        if(this->ctx.selectedItem < this->getMaxIndex()){
            this->ctx.selectedItem++;
            // если курсор совпадает с самым нижним отображающимся элементом
            if (this->ctx.selectedItem == this->getBottomIndex()){
                // если после перемещения курсор не стал совпадать с самым последним элементом
                if (this->ctx.selectedItem < this->getMaxIndex()){
                    //двигаем окно вниз
                    this->ctx.startingIndex++;
                }
            }
            draw();
        }
    }

    void enter(){
        FileInfo f = getSelectedItem();
        if(f.special && f.name==UPDIR){
            if(history.size()){
                DisplayContext restored = history.back();
                history.pop_back();
                setContext(restored);
            }

        } else {
            history.push_back(this->ctx);
            setCWD(f.path);
        }
        draw();
    }
private:
    void setContext(DisplayContext newContext){
        this->setCWD(newContext.cwd);
        this->ctx.selectedItem = newContext.selectedItem;
        this->ctx.startingIndex = newContext.startingIndex;
    }
    string getTypeIcon(FileInfo info) {

        if(S_ISDIR(info.stats.st_mode)){
            return "/";
        }
        if(info.special){
            if(info.name == UPDIR){
                return "/";
            }
        }
        return " ";
    }

    void drawHeader() {
        string b = {HORIZONTAL_BORDER};
        cout << TOP_LEFT_CORNER_BORDER << repeat(b , getDisplayWidth()-2) << TOP_RIGHT_CORNER_BORDER;
    }
    void drawFooter() {
        string b = {HORIZONTAL_BORDER};
        cout << BOTTOM_LEFT_CORNER_BORDER << repeat(b , getDisplayWidth()-2) << BOTTOM_RIGHT_CORNER_BORDER;
    }

    void draw(){
        consoleSelect(&this->printConsole);
        cout << position(0,0) << BG_DEFAULT;
        drawHeader();
        u32 offset = 1;
        for (u32 i = 0; i < this->getDisplayHeight() ; ++i) {
            cout << position(i + offset,0);
            u32 drawingItem = i + this->ctx.startingIndex;
            cout << BG_DEFAULT << VERTICAL_BORDER;
            if(drawingItem == this->ctx.selectedItem && this->active) {
                cout << BG_HIGHLIGHT;
            }
            string filename = "";
            string type = " ";
            if(drawingItem <= this->getMaxIndex()){
                filename = this->getItem(drawingItem).name;
                type = getTypeIcon(this->getItem(drawingItem));
            }

            cout << type << this->prepareFilename(filename) << VERTICAL_BORDER << "     ";
            if(drawingItem == this->ctx.selectedItem && this->active) {
                cout << BG_DEFAULT;
            }
            cout  << VERTICAL_BORDER;
        }
        cout << position(getDisplayHeight() + offset,0) << BG_DEFAULT;
        drawFooter();
    }

    string prepareFilename(string s){
        string s1 = s.substr(0, FILENAME_WIDTH);
        while (s1.length() < FILENAME_WIDTH){
            s1+=" ";
        }
        return s1;
    }

    u32 getDisplayHeight(){
        int ht = this->printConsole.windowHeight - OCCUPIED;
        if (ht < OCCUPIED){
            return 0;
        }
        return (u32)ht;
    }
    u32 getDisplayWidth(){
        int wt = this->printConsole.windowWidth;
        if (wt < 0){
            return 0;
        }
        return (u32)wt;
    }



    u32 getBottomIndex(){
        return this->ctx.startingIndex + this->getDisplayHeight() - 1;
    }

    u32 getMaxIndex(){
        // not doing -1 because we have extra item on top [..]
        return this->items.size();
    }

    PrintConsole printConsole;
    vector<FileInfo> items;
    vector<DisplayContext> history;
    FileInfo updir;
    DisplayContext ctx;
    bool active;
    const int OCCUPIED = 2;
};


#endif //MAIN_EXEC_FILEPANE_H
