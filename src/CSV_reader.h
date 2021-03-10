//
// Created by alex on 10.03.2021.
//

#ifndef NOKIA_CSV_CSV_READER_H
#define NOKIA_CSV_CSV_READER_H

#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <utility>
#include <algorithm>
#include <regex>

class CSV_reader
{
    std::ifstream file_;
    std::vector<std::string> columns_;
    std::vector<std::string> rows_;
    std::vector<std::string> any_cell_;
    std::map<std::string, std::pair<bool, std::string>> shd_matrix_;
public:

    CSV_reader(std::string path);
    void Parse();
    void Read();
    void Write() const;
};


#endif //NOKIA_CSV_CSV_READER_H
