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
#define  UPDIR ".."
template <class T>
inline std::string to_string (const T& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}
using namespace std;
typedef struct dirent dirent;
const u8 FILENAME_WIDTH = 16;
struct FileInfo{
    string name;
    string path;
    bool special;
    struct stat stats;
};

vector<FileInfo> list_files(string dir){
    vector<FileInfo> v;
    DIR *dp;
    dirent *dirp;
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


class FilePane {
    // special case:
    // index 0 represents UP-DIR (..)
    // index X represents items[X-1]
public:
    FilePane(PrintConsole printConsole, string cwd) {
        this->printConsole = printConsole;
        this->startingIndex = 0;
        this->selectedItem = 0;
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
    void setSelectedItemIndex(u32 item) {
        this->selectedItem = item;
    }

    u32 getSelectedItemIndex() {
        return this->selectedItem;
    }
    FileInfo getItem(u32 index) {
        if(index == 0){
            return updir;
        } else {
            return this->items[index-1];
        }
    }
    FileInfo getSelectedItem() {
        return getItem(this->selectedItem);
    }

    string getCWD() {
        return this->cwd;
    }

    void setCWD(string cwd) {
        this->cwd = cwd;
        this->items = list_files(this->cwd);
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
        return "?";

    }

    void draw(){
        consoleSelect(&this->printConsole);

        for (u32 i = 0; i < this->getDisplayHeight() ; ++i) {
            cout << position(i,0);
            u32 drawingItem = i + this->startingIndex;
            cout << setColor(COLOR_WHITE, COLOR_BLUE) << "|";
            if(drawingItem == this->selectedItem && this->active) {
                cout << setColor(COLOR_BLACK, COLOR_CYAN);
            }
            string filename = "";
            string type = " ";
            if(drawingItem <= this->getMaxIndex()){
                filename = this->getItem(drawingItem).name;
                type = getTypeIcon(this->getItem(drawingItem));
            }

            cout << type << this->prepareFilename(filename) << "|" << "     ";
            if(drawingItem == this->selectedItem && this->active) {
                cout << setColor(COLOR_WHITE, COLOR_BLUE);
            }
            cout  << "|";
        }
    }
    void moveUp(){
        if(this->selectedItem > 0){
            this->selectedItem--;
            // если курсор совпадает с самым верхним отображающимся элементом
            if (this->selectedItem == this->startingIndex){
                // если после перемещения курсор не стал совпадать с самым первым элементом
                if (this->selectedItem > 0){
                    //двигаем окно вверх
                    this->startingIndex = this->selectedItem-1;
                }
            }
            draw();
        }
    }
    void moveDown(){
        if(this->selectedItem < this->getMaxIndex()){
            this->selectedItem++;
            // если курсор совпадает с самым нижним отображающимся элементом
            if (this->selectedItem == this->getBottomIndex()){
                // если после перемещения курсор не стал совпадать с самым последним элементом
                if (this->selectedItem < this->getMaxIndex()){
                    //двигаем окно вниз
                    this->startingIndex++;
                }
            }
            draw();
        }
    }
    void enter(){

    }
private:
    string prepareFilename(string s){
        string s1 = s.substr(0, FILENAME_WIDTH);
        while (s1.length() < FILENAME_WIDTH){
            s1+=" ";
        }
        return s1;
    }
    u32 getDisplayHeight(){
        int ht = this->printConsole.windowHeight;
        if (ht < 0){
            return 0;
        }
        return (u32)ht;
    };
    u32 getBottomIndex(){
        return this->startingIndex + this->getDisplayHeight() - 1;
    }
    u32 getMaxIndex(){
        // not doing -1 because we have extra item on top [..]
        return this->items.size();
    }
    PrintConsole printConsole;
    vector<FileInfo> items;
    FileInfo updir;
    u32 selectedItem;
    u32 startingIndex;
    string cwd;
    bool active;
};


#endif //MAIN_EXEC_FILEPANE_H
