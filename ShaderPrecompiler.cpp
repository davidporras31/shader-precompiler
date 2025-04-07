#include "ShaderPrecompiler.h"

void ShaderPrecompiler::precompileShader(std::string source, std::string destination, std::forward_list<std::pair<std::string, std::string>> *defines)
{
    for (auto &&i : *defines)
    {
        define[i.first] = i.second;
    }
    std::ofstream ofs(destination, std::ofstream::out);

    include(ofs, source);

    ofs.close();

    define.clear();
}
void ShaderPrecompiler::include(std::ofstream &out_file, std::string source)
{
    std::ifstream ifs(source, std::ifstream::in);
    std::string s, path;
    path = source.substr(0, source.find_last_of('/') + 1);
    char c;
    while (ifs.get(c))
    {
        if (isEndOfToken(c))
        {
            if (s.front() == '#')
            {
                processPrecompileStatement(out_file, ifs, path, s);
                s.clear();
                continue;
            }
            else if (isInDefine(s))
            {
                processDefine(out_file, s, c);
                s.clear();
                continue;
            }
            else if (c == '(' && isInMacro(s))
            {
                processMacro(out_file, ifs, s);
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
            processPrecompileStatement(out_file, ifs, path, s);
            s.clear();
        }
        else if (isInDefine(s))
        {
            processDefine(out_file, s, c);
            s.clear();
        }
        else if (c == '(' && isInMacro(s))
        {
            processMacro(out_file, ifs, s);
            s.clear();
        }
    }

    if (isEndOfToken(c))
    {
        out_file << s;
    }
    ifs.close();
}

void ShaderPrecompiler::processPrecompileStatement(std::ofstream &out_file, std::ifstream &in_file, std::string path, std::string token)
{
    if (token == "#define")
    {
        std::string name, value;
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
            if (tmp == '\n')
            {
                out_file << tmp;
                break;
            }
            name += tmp;
        }
        if (tmp != '\n')
        {
            while (in_file.get(tmp))
            {
                if (tmp != ' ')
                {
                    break;
                }
            }
        }
        if (tmp != '\n')
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
            if (tmp == '\n')
            {
                out_file << tmp;
                break;
            }
            pathn += tmp;
        }
        path = path + pathn.substr(1, pathn.length() - 2);
        include(out_file, path);
        out_file << tmp;
    }
    if (token == "#ifdef")
    {
        std::string name;
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
            if (tmp == ' ' || tmp == '\n')
            {
                break;
            }
            name += tmp;
        }
        if (!isInDefine(name))
            skipToEndIf(out_file, in_file);
    }
    if (token == "#ifndef")
    {
        std::string name;
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
            if (tmp == ' ' || tmp == '\n')
            {
                break;
            }
            name += tmp;
        }
        if (isInDefine(name))
            skipToEndIf(out_file, in_file);
    }
    if (token == "#else")
    {
        skipToEndIf(out_file, in_file);
    }
}
void ShaderPrecompiler::processDefine(std::ofstream &out_file, std::string token, char end)
{
    out_file << define[token];
    if (isEndOfToken(end))
        out_file << end;
}
void ShaderPrecompiler::processMacro(std::ofstream &out_file, std::ifstream &in_file, std::string token)
{
    for (auto &&i : define)
    {
        size_t p = i.first.find_last_of('(');
        if (p == std::string::npos)
            continue;
        std::string s = i.first.substr(0, p);
        if (s == token)
        {
            std::forward_list<std::string> params;
            std::forward_list<std::string> values;
            std::string param = i.first.substr(s.length() + 1);
            std::string value;
            param = param.substr(0, param.length() - 1);
            splitString(param, ',', &params);
            char c;
            while (in_file.get(c))
            {
                if (c == ')')
                    break;
                value += c;
            }
            splitString(value, ',', &values);
            if (std::distance(values.begin(), values.end()) != std::distance(params.begin(), params.end()))
            {
                throw MacroParamExecption();
            }
            auto it1 = values.begin();
            auto it2 = params.begin();
            std::string out = i.second;
            while (it1 != values.end() && it2 != params.end())
            {
                std::regex e(*it2);
                out = std::regex_replace(out, e, *it1);
                ++it1;
                ++it2;
            }
            out_file << out;
            break;
        }
    }
    return;
}
std::string ShaderPrecompiler::stripFile(std::string path)
{
    return path.substr(0, path.find_last_of('\\') + 1);
}
bool ShaderPrecompiler::isInDefine(std::string name)
{
    return define.find(name) != define.end();
}
bool ShaderPrecompiler::isInMacro(std::string name)
{
    for (auto &&i : define)
    {
        size_t p = i.first.find_last_of('(');
        if (p == std::string::npos)
            continue;
        std::string s = i.first.substr(0, p);
        if (s == name)
            return true;
    }

    return false;
}
bool ShaderPrecompiler::isEndOfToken(char c)
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
void ShaderPrecompiler::skipToEndIf(std::ofstream &out_file, std::ifstream &in_file)
{
    std::string s;
    char c;
    unsigned int nb_if = 0;
    while (in_file.get(c))
    {
        if (isEndOfToken(c))
        {
            if (s == "#endif")
            {
                if (nb_if == 0)
                    break;
                else
                    --nb_if;
            }
            if (s == "#ifdef" || s == "ifndef")
                ++nb_if;
            s.clear();
        }
        else
        {
            s += c;
        }
    }
    out_file << c;
}

void ShaderPrecompiler::splitString(std::string string, char delimiter, std::forward_list<std::string> *list)
{
    size_t pos = 0;
    std::string token;
    while ((pos = string.find(delimiter)) != std::string::npos)
    {
        token = string.substr(0, pos);
        list->push_front(token);
        string.erase(0, pos + 1);
    }
    list->push_front(string);
}