//
// Created by dram on 07.05.18.
//

#ifndef FILEEXCHANGER_DATABASE_H
#define FILEEXCHANGER_DATABASE_H

#include <string>
#include <fstream>
#include <utility>
#include <stdexcept>
#include <vector>
#include <sstream>
#include <iostream>

class DataBase {
public:
    DataBase(const std::string& fileName) : fileName(fileName){
        fStream.open(fileName.c_str(), std::ios::out); // create file if no exists
        fStream.close();
        fStream.open(fileName.c_str(), std::ios::in | std::ios::out); // create file if no exists

        if(!fStream.good()){
            throw std::runtime_error("Could not open file!");
        }

    }
    std::string getKey(const std::string& key){

        fStream.clear();
        fStream.seekp(0, std::ios::beg);
        fStream.seekg(0, std::ios::beg);
        std::string line;
        while(!fStream.eof()){
            getline(fStream, line);
            std::vector<std::string> result = explode(line, '=');
            if(result[0] == key){
                return result[1];
            }
        }
        return NULL;
    }
    void saveKey(const std::string& key, const std::string& value){

        //buff whole file, and remove value
        fStream.clear();
        fStream.seekp(0, std::ios::beg);
        fStream.seekg(0, std::ios::beg);
        std::stringstream buff;
        std::string line;
        //first remove key
        while(!fStream.eof()){
            getline(fStream, line);
            std::vector<std::string> result = explode(line, '=');
            if(result.size() == 2 && result[0] != key){
                buff << line << std::endl;
            }
        }

        //open file again with truncate
        fStream.close();
        fStream.open(fileName.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
        fStream << buff.str();
        fStream << key << "="<<value << std::endl;
        fStream.flush();
    }
    ~DataBase(){
        fStream.close();
    }
private:
    std::string fileName;
    std::fstream fStream;
    std::vector<std::string> explode(std::string const & s, char delim)
    {
        std::vector<std::string> result;
        std::istringstream iss(s);

        for (std::string token; std::getline(iss, token, delim); )
        {
            result.push_back(token);
        }

        return result;
    }

};
#endif //FILEEXCHANGER_DATABASE_H
