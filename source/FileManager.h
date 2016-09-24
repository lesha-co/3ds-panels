//
// Created by lichevsky on 24.09.16.
//

#ifndef MAIN_EXEC_FILEMANAGER_H
#define MAIN_EXEC_FILEMANAGER_H
#include <string>
class FilePane;
#include "FilePane.h"
using namespace std;

class FileManager {
public:
    FileManager(string cwd_left, string cwd_right);

    ~FileManager();

    void clock(u32 kDown, u32 kHeld);

    PrintConsole* getBottomConsole();
private:
    void setupConsoles();

    FilePane* l;
    FilePane* r;
    FilePane* active;
    PrintConsole menu, leftpan, rightpan, under_panels, actions, context, bottom;
};


#endif //MAIN_EXEC_FILEMANAGER_H
