
#include "../ShaderPrecompiler.h"
int main()
{
    //std::cout << "hello";
    std::forward_list<std::pair<std::string,std::string>> define ;
    define.push_front({"mah","15"});
    precompileShader("test/in.txt","test/out.txt",&define);
    return 1;
}