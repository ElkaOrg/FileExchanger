//
// Created by dram on 09.05.18.
//

#ifndef FILEEXCHANGER_DIRMANAGMENT_H
#define FILEEXCHANGER_DIRMANAGMENT_H

#include <string>
#include <memory>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/functional/hash.hpp>

using namespace boost::filesystem;
/* ex usage */
/**
DirManagment dir("upload/");

std::cout << dir.calculateDirHash() << std::endl;
*/
class DirManagment {
public:
    explicit DirManagment();

    DirManagment(const std::string &dirPathName) : dirPathName(dirPathName), dirPath(dirPathName) {
        if (!is_directory(dirPath)) {
            if (dirPathName.size() > 0) {
                boost::filesystem::create_directory(dirPathName);
            } else {
                throw std::runtime_error("This is not a valid directory!");
            }
        }

    }

    std::vector<std::string> getAllFileNames() {
        directory_iterator end_itr;
        std::vector<std::string> result;

        // loop over iterator
        for (directory_iterator itr(dirPath); itr != end_itr; ++itr) {
            //check if this is file
            if (is_regular_file(itr->path())) {
                result.push_back(itr->path().filename().string());
            }
        }
        return result;
    }

    std::vector<path> getAllFilesPath() {
        directory_iterator end_itr;
        std::vector<path> result;

        // loop over iterator
        for (directory_iterator itr(dirPath); itr != end_itr; ++itr) {
            //check if this is file
            if (is_regular_file(itr->path())) {
                result.push_back(itr->path());
            }
        }
        return result;
    }

    std::size_t calculateDirHash() const {
        directory_iterator end_itr;
        std::string fileNamesAndSizes = "";

        // loop over iterator
        for (directory_iterator itr(dirPath); itr != end_itr; ++itr) {
            //check if this is file
            if (is_regular_file(itr->path())) {
                fileNamesAndSizes +=
                        itr->path().filename().string() + std::to_string(boost::filesystem::file_size(itr->path()));
            }
        }
        boost::hash<std::string> string_hash;

        return string_hash(fileNamesAndSizes);
    }

    static bool isValidDirectory(const std::string &path) {
        return is_directory(path);
    }

    static bool isValidFile(const std::string &path) {
        return is_regular_file(path);
    }

    ~DirManagment() {
    }

private:
    const std::string dirPathName;
    const path dirPath;

};

#endif //FILEEXCHANGER_DIRMANAGMENT_H
