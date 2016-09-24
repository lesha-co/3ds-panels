//
// Created by lichevsky on 24.09.16.
//
#include "FilePane.h"
template <class T>
string to_string (const T& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}
string getSizeString(u32 size){
    string prefixes[5] = {"","K","M","G","T"};
    u8 idx = 0;
    while(size >= 1024){
        idx++;
        size /= 1024;
    }
    if (idx>4){
        return "!";
    } else {
        return to_string(size)+prefixes[idx];
    }
}
string leftpad(string s, u32 width){
    string s1 = s.substr(0, width);
    while (s1.length() < width){
        s1+=" ";
    }
    return s1;
}
string rightpad(string s, u32 width){
    string s1 = s.substr(0, width);
    while (s1.length() < width){
        s1=" "+s1;
    }
    return s1;
}

string repeat(string in, u32 times){
    string s = "";
    for (u32 i = 0; i < times; ++i) {
        s += in;
    }
    return s;
}
vector<FileInfo> list_files(string dir, PrintConsole* printConsole){
    vector<FileInfo> v;
    consoleInit(GFX_BOTTOM, printConsole);
    printf("Listing dir %s", dir.c_str());
    if (dir != ""){
        FileInfo up;
        up.name = UPDIR;
        up.special = true;
        v.push_back(up);
    }
    DIR *dp;
    dirent *dirp;

    dp = opendir(dir.c_str() );
    if (dp == NULL)
    {
        return v;
    } else {
        while ((dirp = readdir( dp )))
        {
            FileInfo fi;
            fi.name = dirp->d_name;
            fi.path = dir + "/" + dirp->d_name;
            stat( fi.path.c_str(), &fi.stats );

            v.push_back(fi);
        }
        closedir( dp );
    }
    return v;
}

FilePane::FilePane(PrintConsole printConsole, string cwd, FileManager* fm) {
    //this->printConsole = printConsole;
    //this->active = false;
    //this->setCWD(cwd);
    //this->draw();
    this->fm = fm;
}

void FilePane::setActive(bool active){
    this->active = active;
    //draw();
}

FileInfo FilePane::getItem(u32 index) {
    return this->items[index];

}

FileInfo FilePane::getSelectedItem() {
    return getItem(this->ctx.selectedItem);
}

string FilePane::getCWD() {
    return this->ctx.cwd;
}

void FilePane::setCWD(string cwd) {
    this->ctx.cwd = cwd;
    this->items = list_files(cwd, fm->getBottomConsole());
    this->ctx.selectedItem = 0;
    this->ctx.startingIndex = 0;
}

void FilePane::moveUp(){
    if(this->ctx.selectedItem > 0){
        this->ctx.selectedItem--;
        // если курсор совпадает с самым верхним отображающимся элементом
        if (this->ctx.selectedItem == this->ctx.startingIndex){
            // если после перемещения курсор не стал совпадать с самым первым элементом
            if (this->ctx.selectedItem > 0){
                //двигаем окно вверх
                this->ctx.startingIndex = this->ctx.selectedItem-1;
            }
        }
        //draw();
    }
}

void FilePane::moveDown(){
    if(this->ctx.selectedItem < this->getMaxIndex()){
        this->ctx.selectedItem++;
        // если курсор совпадает с самым нижним отображающимся элементом
        if (this->ctx.selectedItem == this->getBottomIndex()){
            // если после перемещения курсор не стал совпадать с самым последним элементом
            if (this->ctx.selectedItem < this->getMaxIndex()){
                //двигаем окно вниз
                this->ctx.startingIndex++;
            }
        }
        //draw();
    }
}

void FilePane::moveTop(){
    this->ctx.selectedItem = 0;
    this->ctx.startingIndex = 0;
    //draw();
}
void FilePane::moveEnd(){
    this->ctx.selectedItem = this->getMaxIndex();
    if (this->getNumberOfItems() < this->getDisplayHeight()){
        this->ctx.startingIndex = 0;
    } else {
        this->ctx.startingIndex = this->ctx.selectedItem - this->getDisplayHeight() + 1;
    }

    //draw();
}
void FilePane::updir(){
    if(history.size()){
        DisplayContext restored = history.back();
        history.pop_back();
        setContext(restored);
        //draw();
    }
}
void FilePane::enter(){
    FileInfo f = getSelectedItem();
    if(f.special && f.name==UPDIR){
        updir();
    } else {
        if(S_ISDIR(f.stats.st_mode)){
            history.push_back(this->ctx);
            setCWD(f.path);
            //draw();
        }

    }
}
//
void FilePane::setContext(DisplayContext newContext){
    this->setCWD(newContext.cwd);
    this->ctx.selectedItem = newContext.selectedItem;
    this->ctx.startingIndex = newContext.startingIndex;
}
//
//string FilePane::getSupplementaryInfo(FileInfo info){
//    if(info.special){
//        if(info.name == UPDIR){
//            return "UPDIR";
//        }
//    }
//    if(S_ISDIR(info.stats.st_mode)){
//        return "     ";
//    }
//    return getSizeString((u32)info.stats.st_size);
//}
//
//string FilePane::getTypeIcon(FileInfo info) {
//
//    if(info.special){
//        if(info.name == UPDIR){
//            return UPWARDS_ARROW;
//        }
//    }
//    if(S_ISDIR(info.stats.st_mode)){
//        return "/";
//    }
//    return " ";
//}
//
//void FilePane::drawHeader() {
//    string b = {HORIZONTAL_BORDER};
//    cout << TOP_LEFT_CORNER_BORDER << repeat(b , getDisplayWidth()-2) << TOP_RIGHT_CORNER_BORDER;
//}
//
//void FilePane::drawFooter() {
//    string b = {HORIZONTAL_BORDER};
//    cout << BOTTOM_LEFT_CORNER_BORDER << repeat(b , getDisplayWidth()-2) << BOTTOM_RIGHT_CORNER_BORDER;
//}
//
//void FilePane::draw(){
//
//    consoleSelect(&this->printConsole);
//    cout << position(0,0) << BG_DEFAULT;
//    drawHeader();
//    // number of lines we need to skip from the top of under_panels ( top border in this case)
//    u32 offset = 1;
//    for (u32 i = 0; i < this->getDisplayHeight() ; ++i) {
//        cout << position(i + offset,0);
//        u32 drawingItem = i + this->ctx.startingIndex;
//        cout << BG_DEFAULT << VERTICAL_BORDER;
//        bool is_selected = (drawingItem == this->ctx.selectedItem && this->active);
//        if(is_selected) {
//            cout << BG_HIGHLIGHT;
//            consoleSelect(fm->getBottomConsole());
//            consoleClear();
//            printf("\nStat for file %s\n", this->getItem(drawingItem).name.c_str());
//            printf("Path: %s\n", this->getItem(drawingItem).path.c_str());
//            printf("Mode: %d\n", this->getItem(drawingItem).stats.st_mode);
//            printf("Size: %lu\n", (u32)this->getItem(drawingItem).stats.st_size);
//            consoleSelect(&this->printConsole);
//        }
//        string filename = "";
//        string type = " ";
//        string supplementary = "     ";
//        if(drawingItem <= this->getMaxIndex()){
//            filename = this->getItem(drawingItem).name;
//            type = getTypeIcon(this->getItem(drawingItem));
//            supplementary = getSupplementaryInfo(this->getItem(drawingItem)).substr(0,5);
//        }
//
//        cout << type << leftpad(filename, FILENAME_WIDTH) << VERTICAL_BORDER << rightpad(supplementary, 5);
//        if(is_selected) {
//            cout << BG_DEFAULT;
//        }
//        cout  << VERTICAL_BORDER;
//    }
//    cout << position(getDisplayHeight() + offset,0) << BG_DEFAULT;
//    drawFooter();
//}

u32 FilePane::getDisplayHeight(){
    int ht = this->printConsole.windowHeight - OCCUPIED;
    if (ht < OCCUPIED){
        return 0;
    }
    return (u32)ht;
}
//
//u32 FilePane::getDisplayWidth(){
//    int wt = this->printConsole.windowWidth;
//    if (wt < 0){
//        return 0;
//    }
//    return (u32)wt;
//}
//
u32 FilePane::getBottomIndex(){
    return this->ctx.startingIndex + this->getDisplayHeight() - 1;
}
//
u32 FilePane::getMaxIndex(){
    return getNumberOfItems()-1;
}
//
u32 FilePane::getNumberOfItems(){
    return items.size();
}