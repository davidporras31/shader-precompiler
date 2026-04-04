#include <iostream>
#include <assert.h>
#include "../ShaderPrecompiler.h"
int main()
{
    std::cout << "start" << std::endl;
    std::cout <<  "precompiling..." << std::endl;
    std::map<std::string, std::string> define;
    define["val1"] = "";
    define["val2"] = "pass";
    define["macro1(test)"] = "test";
    ShaderPrecompiler::precompileShader("test/in.txt", "test/out.txt", &define);

    //test if define don't change
    define["val1"] = "";
    define["val2"] = "pass";
    define["macro1(test)"] = "test";
    assert(ShaderPrecompiler::needsReprecompile("test/out.txt",&define)==false);
    //test if define empty
    define.clear();
    assert(ShaderPrecompiler::needsReprecompile("test/out.txt",&define)==true);
    //test if define changed
    define["val1"] = "boop";
    define["val2"] = "pass";
    define["macro1(test)"] = "test";
    assert(ShaderPrecompiler::needsReprecompile("test/out.txt",&define)==true);
    std::cout << "end" << std::endl;
    return 0;
}