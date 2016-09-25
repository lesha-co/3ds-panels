//
// Created by lichevsky on 25.09.16.
//

#ifndef MAIN_EXEC_DISKOPERATION_H
#define MAIN_EXEC_DISKOPERATION_H

#include <string>
#include <dirent.h>
#include <cstdio>
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
private:
    DiskOperationType type;
    std::string path_from;
    std::string path_to;
};


#endif //MAIN_EXEC_DISKOPERATION_H
