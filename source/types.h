//
// Created by lichevsky on 24.09.16.
//

#ifndef MAIN_EXEC_TYPES_H
#define MAIN_EXEC_TYPES_H
#include "includes.h"
typedef struct dirent dirent;
typedef struct stat s_stat;

struct FileInfo{
    std::string name;
    std::string path;
    bool special;
    s_stat stats;
};
struct DisplayContext{
    std::string cwd;
    u32 selectedItem = 0;
    u32 startingIndex = 0;
};

enum DisplayMode_t{
    MODE_NORMAL = 0,
    MODE_PROMPT_DELETE = 1
};
#endif //MAIN_EXEC_TYPES_H
