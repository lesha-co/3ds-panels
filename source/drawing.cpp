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