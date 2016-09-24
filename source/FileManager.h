//
// Created by lichevsky on 24.09.16.
//

#ifndef MAIN_EXEC_FILEMANAGER_H
#define MAIN_EXEC_FILEMANAGER_H
#include <string>
#include "types.h"
class FilePane;
#include "FilePane.h"
using namespace std;

class FileManager {
public:
    FileManager(string cwd_left, string cwd_right);

    ~FileManager();

    void clock(u32 kDown, u32 kHeld);

    PrintConsole* getBottomConsole();

    vector<FileInfo> list_files(string dir);

    void onCWDUpdate(FilePane* pane);

    void DeleteFile(string path);
private:
    void setupConsoles();

    FilePane* l;
    FilePane* r;
    FilePane* active;
    PrintConsole menu, leftpan, rightpan, under_panels, actions, bottom, prompt;
    DisplayMode_t mode;

    void setmode(DisplayMode_t mode);
};


#endif //MAIN_EXEC_FILEMANAGER_H
