//
// Created by lichevsky on 25.09.16.
//

#include "DiskOperation.h"
using namespace std;

DiskOperation::DiskOperation(DiskOperationType type, string path_from, string path_to){
    this->type = type;
    this->path_from = path_from;
    this->path_to = path_to;
    this->finished = false;
    this->started = false;
    this->files_opened = false;
}

void DiskOperation::copy_tick() {
    if(this->files_opened){
        size_t size;
        size = fread(this->buf, 1, BUFFER_SIZE, this->source);
        if (size <= 0){
            fclose(this->source);
            fclose(this->destination);
            this->finished = true;
        } else{
            fwrite(this->buf, 1, size, this->destination);
        }
    } else {
        this->source = fopen(this->path_from.c_str(), "rb");
        this->destination = fopen(this->path_to.c_str(), "wb");
        fseek(this->source, 0L, SEEK_END);
        this->OperationSize = ftell(this->source);
        rewind(this->source);
        this->files_opened = true;
    }
}
/**
 *
 * @return false if finished, true if not finished
 */

void DiskOperation::tick(){
    if(!this->started || this->finished)
        return;
    switch (this->type){
        case COPY:{
            copy_tick();
            break;
        }
        case DELETE:{
            remove(this->path_from.c_str());
            this->finished = true;
            break;
        }
        case MOVE:{
            rename(this->path_from.c_str(),
                   this->path_to.c_str());
            this->finished = true;
            break;
        }
    }
}
void DiskOperation::commence(){
    this->started = true;
}
bool DiskOperation::is_finished(){
    return this->finished;
}
bool DiskOperation::is_started(){
    return this->started;
}
double DiskOperation::get_progress(){
    if(!this->started || !this->files_opened){
        return -1;
    }
    if(this->finished){
        return 1;
    }
    return (double)ftell(this->source) / (double)this->OperationSize;
}