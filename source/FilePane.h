//
// Created by lichevsky on 20.09.16.
//

#ifndef MAIN_EXEC_FILEPANE_H
#define MAIN_EXEC_FILEPANE_H


#include <3ds.h>
#include <string>
#include <vector>
#include <sys/stat.h>

using namespace std;

struct FileInfo{
    string name;
    string path;
    struct stat stats;
};

class FilePane {
public:
    FilePane(PrintConsole printConsole) {
        this->printConsole = printConsole;
    }

    void setSelectedItemIndex(u32 item) {

    }

    u32 getSelectedItemIndex() {
        return 0;
    }

    string getSelectedItem() {
        return std::__cxx11::string();
    }

    string getCWD() {
        return std::__cxx11::string();
    }

    void setCWD(string cwd) {

    }

private:
    PrintConsole printConsole;
    vector<FileInfo> items;
    //static const string fmt = "|@bzcmp           | 353M|";
};


#endif //MAIN_EXEC_FILEPANE_H
