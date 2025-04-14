#include <iostream>
#include "../ShaderPrecompiler.h"
int main()
{
    std::cout << "start" << std::endl;
    std::map<std::string, std::string> define;
    define["val1"] = "";
    define["val2"] = "pass";
    define["macro1(test)"] = "test";
    ShaderPrecompiler::precompileShader("test/in.txt", "test/out.txt", &define);
    std::cout << "end" << std::endl;
    return 0;
}