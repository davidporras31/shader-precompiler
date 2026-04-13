# shader precompiler

 a c style precompiler for text file like shader source code,
 requires c++17 minimum

## featured preprocessor directive

- #define
- #include
- #ifdef
- #ifndef
- #undef
- #error

## feature

- macro
- file inclusion and recursive file inclusion
- error statement
- dependency file for checking if the destination file need to be reprocessed

## usage

```cpp
#include "ShaderPrecompiler.h"

std::map<std::string, std::string> define;
define["val2"] = "pass";
if(ShaderPrecompiler::needsReprecompile("output/test.glsl", &define))   //check if the destination file need to be reprocessed using the destination dependency file
    ShaderPrecompiler::precompileShader("input/test.glsl", "output/test.glsl", &define);
```
