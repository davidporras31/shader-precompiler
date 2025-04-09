#include <iostream>
#include "../ShaderPrecompiler.h"
int main()
{
    std::cout << "start" << std::endl;
    std::forward_list<std::pair<std::string, std::string>> define;
    define.push_front({"val1", ""});
    define.push_front({"val2", "pass"});
    define.push_front({"macro1(test)", "test"});
    ShaderPrecompiler::precompileShader("test/in.txt", "test/out.txt", &define);
    std::cout << "end" << std::endl;
    return 0;
}