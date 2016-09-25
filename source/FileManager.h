//
// Created by lichevsky on 24.09.16.
//

#ifndef MAIN_EXEC_FILEMANAGER_H
#define MAIN_EXEC_FILEMANAGER_H
class FilePane;

#include "3ds.h"
#include "FilePane.h"
#include "DiskOperation.h"
#include "types.h"
#include <vector>
#include <string>

enum Menu_t {
    MENU_COPY,
    MENU_MOVE,
    MENU_DELETE
};
class FileManager {
public:
    FileManager(std::string cwd_left, std::string cwd_right);

    ~FileManager();

    void clock(u32 kDown, u32 kHeld);

    PrintConsole* getBottomConsole();

    std::vector<FileInfo> list_files(std::string dir);

    void onCWDUpdate(FilePane* pane);

private:
    void setupConsoles();

    FilePane* l;
    FilePane* r;
    FilePane* active;
    PrintConsole menu, leftpan, rightpan, under_panels, actions, bottom,
        prompt, prompt_body;
    DisplayMode_t mode;
    std::vector<Menu_t> menuConfig;
    u32 selectedMenuItem;
    void preparePrompt(bool selectBody);
    void drawMenu();
    FilePane* getInactivePane();
    void setmode(DisplayMode_t mode);
};


#endif //MAIN_EXEC_FILEMANAGER_H
