//
// Created by lichevsky on 20.09.16.
//

#ifndef MAIN_EXEC_FILEPANE_H
#define MAIN_EXEC_FILEPANE_H
class FileManager;
#include <3ds.h>
#include <string>
#include <vector>
#include <sstream>
#include <cstdio>
#include <dirent.h>
#include <iostream>
#include "../3ds_string_utils/source/string_utils.h"
#include "FileManager.h"

using namespace std;

#define  UPDIR ".."
const string VERTICAL_BORDER = {(char)0xB3};
const string HORIZONTAL_BORDER = {(char)0xC4};
const string TOP_LEFT_CORNER_BORDER = {(char)0xDA};
const string TOP_RIGHT_CORNER_BORDER = {(char)0xBF};
const string BOTTOM_LEFT_CORNER_BORDER = {(char)0xC0};
const string BOTTOM_RIGHT_CORNER_BORDER = {(char)0xD9};
const string UPWARDS_ARROW = {(char)0x18};
const u8 FILENAME_WIDTH = 16;
const string BG_DEFAULT = setColor(COLOR_WHITE, COLOR_BLUE);
const string BG_HIGHLIGHT = setColor(COLOR_BLACK, COLOR_CYAN);

typedef struct dirent dirent;
typedef struct stat s_stat;

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

template <class T>
string to_string (const T& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}
string getSizeString(u32 size){
    string prefixes[5] = {"","K","M","G","T"};
    u8 idx = 0;
    while(size >= 1024){
        idx++;
        size /= 1024;
    }
    if (idx>4){
        return "!";
    } else {
        return to_string(size)+prefixes[idx];
    }
}
string leftpad(string s, u32 width){
    string s1 = s.substr(0, width);
    while (s1.length() < width){
        s1+=" ";
    }
    return s1;
}
string rightpad(string s, u32 width){
    string s1 = s.substr(0, width);
    while (s1.length() < width){
        s1=" "+s1;
    }
    return s1;
}

string repeat(string in, u32 times){
    string s = "";
    for (u32 i = 0; i < times; ++i) {
        s += in;
    }
    return s;
}

vector<FileInfo> list_files(string dir, PrintConsole* printConsole){
    vector<FileInfo> v;
    consoleInit(GFX_BOTTOM, printConsole);
    printf("Listing dir %s", dir.c_str());
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


class FilePane {
public:
    FilePane(PrintConsole printConsole, string cwd, FileManager* fm);

    void setActive(bool active);

    FileInfo getItem(u32 index);

    FileInfo getSelectedItem();

    string getCWD();

    void setCWD(string cwd);

    void moveUp();

    void moveDown();

    void moveTop();

    void moveEnd();

    void updir();

    void enter();
private:
    void setContext(DisplayContext newContext);

    string getSupplementaryInfo(FileInfo info);
    
    string getTypeIcon(FileInfo info);

    void drawHeader();

    void drawFooter();

    void draw();

    u32 getDisplayHeight();

    u32 getDisplayWidth();

    u32 getBottomIndex();

    u32 getMaxIndex();

    u32 getNumberOfItems();

    PrintConsole printConsole;
    vector<FileInfo> items;
    vector<DisplayContext> history;
    DisplayContext ctx;
    FileManager* fm;
    bool active;
    // # of lines that occupied by things that are not list of files
    const int OCCUPIED = 2;
};


#endif //MAIN_EXEC_FILEPANE_H
