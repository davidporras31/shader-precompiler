#include <iostream>
#include "../ShaderPrecompiler.h"
int main()
{
    std::cout << "start" << std::endl;
    std::forward_list<std::pair<std::string,std::string>> define;
    define.push_front({"mah","15"});
    define.push_front({"include","boom"});
    define.push_front({"PI","3.14"});
    precompileShader("test/in.txt","test/out.txt",&define);
    return 0;
}