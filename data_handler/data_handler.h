#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <iostream>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

void readInput();
void writeFile(const std::string& filename, const std::string& content);
void readFile();

#endif

