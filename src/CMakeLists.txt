aux_source_directory (. SRC_LIST)

# PROJECT_SOURCE_DIR/CMAKE_SOURCE_DIR/_SOURCE_DIR: 为包含 PROJECT() 命令的最近一个 CMakeLists.txt 文件所在的文件夹路径
include_directories(${PROJECT_SOURCE_DIR}/include)
link_directories(${PROJECT_SOURCE_DIR}/lib)

set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# find_library: 在指定目录下查找指定库，并把库的绝对路径存放到变量里，其第一个参数是变量名称，第二个参数是库名称，第三个参数是 HINTS，第 4 个参数是路径，其它用法可以参考 cmake 文档
# 使用 find_library 的好处是在执行 cmake … 时就会去查找库是否存在，这样可以提前发现错误，不用等到链接时
# find_library 只能找到静态库吗, 为什么只有 dll 的时候找不到, 是还需要设置什么吗?
find_library(INTERCEPTION_LIB interception ${PROJECT_SOURCE_DIR}/lib)

add_executable (EscModKey_interception ${SRC_LIST})

#  target_link_libraries: 把目标文件与库文件进行链接
target_link_libraries (EscModKey_interception ${INTERCEPTION_LIB})

