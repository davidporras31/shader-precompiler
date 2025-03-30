#include "ShaderPrecompiler.h"

void precompileShader(std::string source, std::string destination, std::pair<std::string, std::string> *defines, size_t nb_defines)
{
    for (size_t i = 0; i < nb_defines; i++)
    {
        define[defines[i].first] = defines[i].second;
    }
    delete[] defines;
    std::ofstream ofs(destination, std::ofstream::out);

    include(ofs, source);

    ofs.close();
}
void include(std::ofstream &out_file, std::string source)
{
    std::ifstream ifs(source, std::ifstream::in);
    std::string s;
    char c;
    while (ifs.get(c))
    {
        if (isEndOfToken(c))
        {
            if (s.front() == '#')
            {
                processPrecompileStatement(out_file, ifs, s, c);
                s.clear();
                continue;
            }
            if (isInDefine(s))
            {
                processDefine(out_file, ifs, s, c);
                s.clear();
                continue;
            }
            s += c;
            out_file << s;
            s.clear();
        }
        else
        {
            s += c;
        }
    }
    if (isEndOfToken(c))
    {
        out_file << s;
    }
    ifs.close();
}

void processPrecompileStatement(std::ofstream &out_file, std::ifstream &in_file, std::string token, char end)
{
    if (token == "#define")
    {
    }
    if (token == "#include")
    {
    }
    if (token == "#ifdef")
    {
    }
    if (token == "#ifndef")
    {
    }
}
std::string stripFile(std::string path)
{
    return path.substr(0, path.find_last_of('\\') + 1);
}
bool isInDefine(std::string name)
{
    return define.find(name) != define.end();
}