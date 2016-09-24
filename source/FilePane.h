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
#include "types.h"

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


class FilePane {
public:
    FilePane(PrintConsole printConsole, FileManager* fm);

    void setActive(bool active);

    FileInfo getItem(u32 index);

    FileInfo getSelectedItem();

    string getCWD();

    void moveUp();

    void moveDown();

    void moveTop();

    void moveEnd();

    void updir();

    void enter();

    void setContext(string cwd);
private:
    void setContext(DisplayContext newContext);

    void setContext(string cwd, u32 selectedItem, u32 startingIndex);

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
