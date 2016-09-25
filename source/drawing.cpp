//
// Created by lichevsky on 25.09.16.
//

#include "drawing.h"
using namespace std;
void drawHeader(BorderSet borderSet, u32 width) {
    string b = {borderSet.HORIZONTAL};
    cout << borderSet.TOP_LEFT_CORNER << repeat(b , width-2) << borderSet.TOP_RIGHT_CORNER;
}

void drawFooter(BorderSet borderSet, u32 width) {
    string b = {borderSet.HORIZONTAL};
    cout << borderSet.BOTTOM_LEFT_CORNER << repeat(b , width-2) << borderSet.BOTTOM_RIGHT_CORNER;
}

void drawBorder(BorderSet borderSet, u32 width, u32 height){
    cout << position(0,0);
    drawHeader(borderSet, width);
    for (int i = 0; i < height - 2; ++i) {
        cout << position(1 + i, 0) << borderSet.VERTICAL << repeat(" ", width-2) << borderSet.VERTICAL ;
    }
    cout << position(height - 1,0);
    drawFooter(borderSet, width);
}