//
// Created by lichevsky on 25.09.16.
//

#ifndef MAIN_EXEC_DRAWING_H
#define MAIN_EXEC_DRAWING_H

#include "includes.h"
struct BorderSet{
    std::string VERTICAL;
    std::string HORIZONTAL;
    std::string TOP_LEFT_CORNER;
    std::string TOP_RIGHT_CORNER;
    std::string BOTTOM_LEFT_CORNER;
    std::string BOTTOM_RIGHT_CORNER;
};
const BorderSet border_single = {
    {(char)0xB3},
    {(char)0xC4},
    {(char)0xDA},
    {(char)0xBF},
    {(char)0xC0},
    {(char)0xD9},
};
const BorderSet border_double = {
    {(char)0xBA},
    {(char)0xCD},
    {(char)0xC9},
    {(char)0xBB},
    {(char)0xC8},
    {(char)0xBC},
};

const std::string BG_DEFAULT = setColor(COLOR_WHITE, COLOR_BLUE);
const std::string BG_HIGHLIGHT = setColor(COLOR_BLACK, COLOR_CYAN);
const std::string BG_PROMPT = setColor(COLOR_WHITE, COLOR_RED);
const std::string UPWARDS_ARROW = {(char)0x18};


#endif //MAIN_EXEC_DRAWING_H
