//
// Created by lichevsky on 25.09.16.
//

#ifndef MAIN_EXEC_DRAWING_H
#define MAIN_EXEC_DRAWING_H

#include <string>
#include "3ds.h"
struct BorderSet{
    string VERTICAL;
    string HORIZONTAL;
    string TOP_LEFT_CORNER;
    string TOP_RIGHT_CORNER;
    string BOTTOM_LEFT_CORNER;
    string BOTTOM_RIGHT_CORNER;
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



#endif //MAIN_EXEC_DRAWING_H
