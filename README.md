# shader precompiler

 precompile shader

## featured preprocessor directive

- #define
- #include
- #ifdef
- #ifndef

## use

```cpp
std::map<std::string, std::string> define;
define["val2"] = "pass";
ShaderPrecompiler::precompileShader("input/test.glsl", "output/test.glsl", &define)
```
