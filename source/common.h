//
// Created by lichevsky on 23.09.16.
//

#ifndef MAIN_EXEC_COMMON_H
#define MAIN_EXEC_COMMON_H

#include <3ds.h>

PrintConsole menu, leftpan, rightpan, console, actions, context, all, bottom;

void drawASCII(){
    consoleSelect(&bottom);
    for (int i = 0; i < 256; ++i) {
        if( i%16 == 0) {
            printf("\n %3x |", i);
        }
        if(i == '\n' || i == '\r' || i == '\0' ){
            printf("X ");
        } else {
            printf("%-2c",i);
        }
    }
}
#endif //MAIN_EXEC_COMMON_H
