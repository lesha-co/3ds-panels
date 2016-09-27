//
// Created by lichevsky on 25.09.16.
//

#ifndef MAIN_EXEC_DISKOPERATION_H
#define MAIN_EXEC_DISKOPERATION_H

#include "3ds.h"
#include <string>
#include <dirent.h>
#include <cstdio>
const u32 BUFFER_SIZE = 32768;
enum DiskOperationType {
    COPY, MOVE, DELETE
};

class DiskOperation {
public:
    DiskOperation(DiskOperationType type,
                  std::string path_from,
                  std::string path_to);

    void commence();
    void tick();
    void abort();
    bool is_finished();
    bool is_started();
    double get_progress();
    std::string getSource();
    std::string getDestination();
private:
    void copy_tick();
    bool finished;
    bool started;
    bool files_opened;
    u32 OperationSize;
    DiskOperationType type;
    std::string path_from;
    std::string path_to;
    FILE* source;
    FILE* destination;
    char buf[BUFFER_SIZE];
};


#endif //MAIN_EXEC_DISKOPERATION_H
