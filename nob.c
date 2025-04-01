#define NOB_IMPLEMENTATION
#include "nob.h"

#define BUILD_FOLDER "build/"
#define OBJECT_FOLDER "obj/"
#define BUILD_ARGS "g++", "-Wall", "-Wextra"

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);
    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;
    if (!nob_mkdir_if_not_exists(OBJECT_FOLDER)) return 1;
    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, BUILD_ARGS , "-c", "-o", OBJECT_FOLDER"ShaderPrecompiler.o" , "ShaderPrecompiler.cpp");
    if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;
    nob_cmd_append(&cmd, BUILD_ARGS, "-o" , BUILD_FOLDER"test.exe" , "test/main.cpp" , OBJECT_FOLDER"ShaderPrecompiler.o" );
    if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;
    
    nob_cmd_append(&cmd , BUILD_FOLDER"test.exe");
    if (!nob_cmd_run_sync_and_reset(&cmd)) return 1;

    return 0;
}

