cmake_minimum_required(VERSION 3.2)

project(demo)

# add_subdirectory：这个语句的作用是增加编译子目录, 其基本语法格式是：
# add_subdirectory(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
# source_dir: 指定一个包含 CMakeLists.txt 和代码文件所在的目录, 该目录可以是绝对路径, 也可以是相对路径, 对于后者相对路径的起点是 CMAKE_CURRENT_SOURCE_DIR, 此外, 如果子目录再次包含的 CMakeLists.txt, 则将继续处理里层的 CMakeLists.txt, 而不是继续处理当前源代码
# binary_dir: 二进制代码目录
# 这个目录是可选的, 如果指定, cmake 命令执行后的输出文件将会存放在此处, 若没有指定, 默认情况等于 source_dir 没有进行相对路径计算前的路径, 也就是 CMAKE_BINARY_DIR
# EXCLUDE_FROM_ALL 标记
# 这个标志是可选的, 如果传递了该参数表示新增加的子目录将会排除在 ALL 目录之外（可能是 make 系统中的 make all？）, 表示这个目录将从 IDE 的工程中排除, 用户必须显式在子文件这个编译目标（手动 cmake 之类的）, 指定了这个文件夹, 表示这个文件夹是独立于源工程的，这些函数是有用但是不是必要的
add_subdirectory(src)
