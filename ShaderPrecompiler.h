#ifndef SHADER_PRECOMPILER_H
#define SHADER_PRECOMPILER_H

#include <string>
#include <map>
#include <forward_list>
#include <array>
#include <fstream>
#include <utility>
#include <iostream>

void precompileShader(std::string source, std::string destination, std::forward_list<std::pair<std::string, std::string>>* defines);
void include(std::ofstream &out_file, std::string source);
void processPrecompileStatement(std::ofstream &out_file, std::ifstream &in_file,std::string path, std::string token, char end);
void processDefine(std::ofstream &out_file, std::ifstream &in_file, std::string token, char end);
std::string stripFile(std::string path);
static std::map<std::string, std::string> define;
bool isInDefine(std::string name);
constexpr std::array<char, 3> end_of_token = {' ', '\n', '('};
bool isEndOfToken(char c);

#endif