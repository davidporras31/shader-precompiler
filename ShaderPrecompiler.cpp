#include "ShaderPrecompiler.h"

void precompileShader(std::string source, std::string destination, std::forward_list<std::pair<std::string, std::string>>* defines)
{
    for (auto &&i : *defines)
    {
        define[i.first] = i.second;
    }
    defines->clear();
    std::ofstream ofs(destination, std::ofstream::out);

    include(ofs, source);

    ofs.close();
}
void include(std::ofstream &out_file, std::string source)
{
    std::ifstream ifs(source, std::ifstream::in);
    std::string s, path;
    path = source.substr( 0, source.find_last_of( '/' ) +1 );
    char c;
    while (ifs.get(c))
    {
        if (isEndOfToken(c))
        {
            if (s.front() == '#')
            {
                processPrecompileStatement(out_file, ifs,path, s, c);
                s.clear();
                continue;
            }else if (isInDefine(s))
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
    if (!s.empty())
    {
        if (s.front() == '#')
        {
            processPrecompileStatement(out_file, ifs,path, s, c);
            s.clear();
        }else if (isInDefine(s))
        {
            processDefine(out_file, ifs, s, c);
            s.clear();
        }
    }
    
    if (isEndOfToken(c))
    {
        out_file << s;
    }
    ifs.close();
}

void processPrecompileStatement(std::ofstream &out_file, std::ifstream &in_file,std::string path, std::string token, char end)
{
    if (token == "#define")
    {
        std::cout << token << end;
        std::string name,value;
        char tmp;
        while (in_file.get(tmp))
        {
            if (tmp != ' ')
            {
                break;
            }
        }
        name += tmp;
        while (in_file.get(tmp))
        {
            if (tmp == ' ')
            {
                break;
            }
            if(tmp == '\n')
            {
                out_file << tmp;
                break;
            }
            name += tmp;
        }
        if(tmp != '\n')
        {
            while (in_file.get(tmp))
            {
                if (tmp != ' ')
                {
                    break;
                }
            }
        }
        if(tmp != '\n')
        {
            value += tmp;
            while (in_file.get(tmp))
            {
                if (tmp == '\n')
                {
                    out_file << tmp;
                    break;
                }
                value += tmp;
            }
        }
        std::cout << name << " " << value << std::endl;
        define[name] = value;
    }
    if (token == "#include")
    {
        std::string pathn;
        char tmp;
        while (in_file.get(tmp))
        {
            if (tmp != ' ')
            {
                break;
            }
        }
        pathn += tmp;
        while (in_file.get(tmp))
        {
            if (tmp == ' ')
            {
                break;
            }
            if(tmp == '\n')
            {
                out_file << tmp;
                break;
            }
            pathn += tmp;
        }
        path = path + pathn.substr(1,pathn.length()-2);
        std::cout << path;
        //include(out_file,path);
    }
    if (token == "#ifdef")
    {
    }
    if (token == "#ifndef")
    {
    }
}
void processDefine(std::ofstream &out_file, std::ifstream &in_file, std::string token, char end)
{
    out_file << define[token];
    if(isEndOfToken(end))
        out_file << end;
}
std::string stripFile(std::string path)
{
    return path.substr(0, path.find_last_of('\\') + 1);
}
bool isInDefine(std::string name)
{
    return define.find(name) != define.end();
}
bool isEndOfToken(char c)
{
    for (auto &&i : end_of_token)
    {
        if (i == c)
        {
            return true;
        }
        
    }
    
    return false;
}