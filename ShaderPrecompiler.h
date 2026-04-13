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
#include <filesystem>

namespace ShaderPrecompiler
{
    /// @brief check if the destination file need to be reprocessed using the destination dependency file
    /// @param destination the destination file
    /// @param defines map of all defines to load before precompiling and use as storage while running
    bool needsReprecompile(std::string destination, std::map<std::string, std::string> *defines);
    /// @brief precompile shader from source to destination and generate a dependency file with the same name as destination and .d extension
    /// @param source the source file use as include source
    /// @param destination the destination file
    /// @param defines map of all defines to load before precompiling and use as storage while running
    /// @exception PrecompilerException if the precompiler read #error statement or can't open source or destination file
    /// @exception MacroParamException if the macro parameter number don't match
    void precompileShader(std::string source, std::string destination, std::map<std::string, std::string> *defines);

    /// @brief include a file to out_file and run precompiler on it and append the dependency to dep_file
    /// @param out_file output file
    /// @param dep_file dependency file
    /// @param source path to the file to open
    /// @param defines map of all defines to load before precompiling and use as storage while running
    /// @exception PrecompilerException if the precompiler read #error statement
    /// @exception MacroParamException if the macro parameter number don't match
    void include(std::ofstream &out_file, std::ofstream &dep_file, std::string source, std::map<std::string, std::string> *defines);
    void processPrecompileStatement(std::ofstream &out_file, std::ofstream &dep_file, std::ifstream &in_file, std::string path, std::string token, std::map<std::string, std::string> *defines);
    void processDefine(std::ofstream &out_file, std::string token, char end, std::map<std::string, std::string> *defines);
    void processMacro(std::ofstream &out_file, std::ifstream &in_file, std::string token, std::map<std::string, std::string> *defines);

    /// @brief strip file from a path
    /// @param path path to strip
    /// @return a path without the file
    std::string stripFile(std::string path);

    bool isInDefine(std::map<std::string, std::string> *defines, std::string name);
    bool isInMacro(std::map<std::string, std::string> *defines, std::string name);
    constexpr std::array<char, 3> end_of_token = {' ', '\n', '('};
    bool isEndOfToken(char c);
    void skipToEndIfOrElse(std::ifstream &in_file);

    /// @brief split string to a list
    /// @param string the string to split
    /// @param delimiter the splitting character
    /// @param list output list
    void splitString(std::string string, char delimiter, std::forward_list<std::string> *list);

    template <typename T, typename V>
    size_t hashAMap(const std::map<T, V> *m)
    {
        size_t seed = 0;
        for (const auto &pair : *m)
        {
            seed ^= std::hash<T>()(pair.first) ^ std::hash<V>()(pair.second);
        }
        return seed;
    }

    /// @brief exception throw when the macro parameter number don't match the definition
    class MacroParamException : public std::exception
    {
    public:
        MacroParamException(size_t given, size_t expected) : m_given(given), m_expected(expected) {}
        const char *what() const throw()
        {
            std::string msg = std::string("macro parameter number don't match use expected ") + std::to_string(m_expected) + " but got " + std::to_string(m_given);
            return msg.c_str();
        }

    private:
        size_t m_given;
        size_t m_expected;
    };

    /// @brief exception throw when the precompiler read #error statement or can't open source or destination file
    class PrecompilerException : public std::exception
    {
    public:
        PrecompilerException(std::string msg) : m_msg(msg) {}
        const char *what() const throw()
        {
            return m_msg.c_str();
        }

    private:
        std::string m_msg;
    };

}
#endif