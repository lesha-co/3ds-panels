//
// Created by lichevsky on 25.09.16.
//

#include "DiskOperation.h"
using namespace std;
void DeleteFile(string path){
    remove(path.c_str());
    //active->redraw();
}
void CopyFile(string from, string to){
    const char* from_path = from.c_str();
    const char* to_path = to.c_str();
    printf("\n\n\t%s", from_path);
    printf("\n\n\t%s", to_path);

    char buf[BUFSIZ];
    size_t size;

    FILE* source = fopen(from_path, "rb");
    FILE* dest = fopen(to_path, "wb");


    printf("\n\n\tstarting...");
    while (true) {
        size = fread(buf, 1, BUFSIZ, source);
        if (size <= 0)
            break;
        fwrite(buf, 1, size, dest);
    }
    fclose(source);
    fclose(dest);

    printf("\n\n\tOK");

}

DiskOperation::DiskOperation(DiskOperationType type, string path_from, string path_to){
    this->type = type;
    this->path_from = path_from;
    this->path_to = path_to;
}

void DiskOperation::tick(){

}
void DiskOperation::commence(){
    switch(this->type){
        case COPY:{
            CopyFile(this->path_from, this->path_to);
            break;
        }
        case MOVE:{
            CopyFile(this->path_from, this->path_to);
            DeleteFile(this->path_from);
            break;
        }
        case DELETE:{
            DeleteFile(this->path_from);
            break;
        }
    }
}