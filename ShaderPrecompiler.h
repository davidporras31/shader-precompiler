#include <string>
#include <map>
#include <array>
#include <fstream>
void precompileShader(std::string source, std::string destination, std::pair<std::string, std::string> *defines, size_t nb_defines);
void include(std::ofstream &out_file, std::string source);
void processPrecompileStatement(std::ofstream &out_file, std::ifstream &in_file, std::string token, char end);
void processDefine(std::ofstream &out_file, std::ifstream &in_file, std::string token, char end);
std::string stripFile(std::string path);
std::map<std::string, std::string> define;
bool isInDefine(std::string name);
std::array<char, 3> end_of_token = {' ', '\n', '('};
bool isEndOfToken(char c);