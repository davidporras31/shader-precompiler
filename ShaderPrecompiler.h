#ifndef SHADER_PRECOMPILER_H
#define SHADER_PRECOMPILER_H

#include <string>
#include <map>
#include <forward_list>
#include <array>
#include <fstream>
#include <utility>
#include <exception>
#include <regex>

namespace ShaderPrecompiler
{
    /// @brief precompile shader from source to destination
    /// @param source the source file use as include source
    /// @param destination the destination file
    /// @param defines map of all defines to load before precompiling and use as storage while runnig
    void precompileShader(std::string source, std::string destination, std::map<std::string, std::string> *defines);

    /// @brief include a file to out_file and run precompiler on it
    /// @param out_file output file
    /// @param source path to the file to open
    void include(std::ofstream &out_file, std::string source, std::map<std::string, std::string> *defines);
    void processPrecompileStatement(std::ofstream &out_file, std::ifstream &in_file, std::string path, std::string token, std::map<std::string, std::string> *defines);
    void processDefine(std::ofstream &out_file, std::string token, char end, std::map<std::string, std::string> *defines);
    void processMacro(std::ofstream &out_file, std::ifstream &in_file, std::string token, std::map<std::string, std::string> *defines);

    /// @brief strip file from a path
    /// @param path path to strip
    /// @return a path without the file
    std::string stripFile(std::string path);

    bool isInDefine( std::map<std::string, std::string> *defines,std::string name);
    bool isInMacro( std::map<std::string, std::string> *defines,std::string name);
    constexpr std::array<char, 3> end_of_token = {' ', '\n', '('};
    bool isEndOfToken(char c);
    void skipToEndIfOrElse(std::ifstream &in_file);

    /// @brief split string to a list
    /// @param string the string to split
    /// @param delimiter the spliting charcter
    /// @param list output list
    void splitString(std::string string, char delimiter, std::forward_list<std::string> *list);

    class MacroParamExecption : public std::exception
    {
    public:
        const char *what() const throw()
        {
            return "macro parameter number don't match use";
        }
    };

}
#endif