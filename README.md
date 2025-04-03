# shader precompiler
 precompile shader
 # featured preprocessor directive:
- #define
- #include
- #ifdef
- #ifndef
# use
```cpp
std::forward_list<std::pair<std::string,std::string>> defines;
defines.push_front({"mah","15"});
void precompileShader("input/test.glsl", "output/test.glsl", &defines)
```
