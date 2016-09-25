//
// Created by lichevsky on 24.09.16.
//

#ifndef MAIN_EXEC_FILEMANAGER_H
#define MAIN_EXEC_FILEMANAGER_H
class FilePane;

#include "includes.h"

class FileManager {
public:
    FileManager(std::string cwd_left, std::string cwd_right);

    ~FileManager();

    void clock(u32 kDown, u32 kHeld);

    PrintConsole* getBottomConsole();

    std::vector<FileInfo> list_files(std::string dir);

    void onCWDUpdate(FilePane* pane);

    void DeleteFile(std::string path);
private:
    void setupConsoles();

    FilePane* l;
    FilePane* r;
    FilePane* active;
    PrintConsole menu, leftpan, rightpan, under_panels, actions, bottom,
        prompt, prompt_body;
    DisplayMode_t mode;

    void setmode(DisplayMode_t mode);
};


#endif //MAIN_EXEC_FILEMANAGER_H
