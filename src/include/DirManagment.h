//
// Created by dram on 09.05.18.
//

#ifndef FILEEXCHANGER_DIRMANAGMENT_H
#define FILEEXCHANGER_DIRMANAGMENT_H

#include <string>
#include <memory>
#include <boost/filesystem.hpp>
using namespace boost::filesystem;

class DirManagment {
public:
    explicit DirManagment();
    DirManagment(const std::string& dirPath) : dirPathName(dirPath){
        path p =  path(dirPath);
        if(!is_directory(p)){
            throw std::runtime_error("This is not a valid directory!");
        }

    }
    ~DirManagment(){
    }
private:
    std::string dirPathName;


};
#endif //FILEEXCHANGER_DIRMANAGMENT_H
