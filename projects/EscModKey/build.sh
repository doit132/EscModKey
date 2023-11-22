CURRENT_DIR=$(pwd)
PATH_WORKSPACE_ROOT=$CURRENT_DIR/../../

CMAKE_PATH="/mnt/d/Programs2/cmake/bin/cmake.exe"
MAKE_PATH="/mnt/d/Programs2/w64devkit/bin/mingw32-make.exe"

if [ ! -d "build" ]; then
    mkdir "build"
fi

cd build

# 这里之所以要判断 CMakeCache.txt 是否存在是为了防止进入的不是 CMake 的目录
if [ "$1" = "clean" ] && [ -f "CMakeCache.txt" ]; then
    yes | rm -rf *
fi

# cmake --build .
if [ -f "Makefile" ]; then
    $MAKE_PATH clean
else
    $CMAKE_PATH -G "MinGW Makefiles" ..
fi

if [ -f "compile_commands.json" ]; then
    cp compile_commands.json $PATH_WORKSPACE_ROOT
fi

$MAKE_PATH -j8

