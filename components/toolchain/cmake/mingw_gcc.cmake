# 编译工具链; 请确保已经添加到环境变量;
# 如果使用的是 linux 环境, 需要将后面的 '.exe' 移除;
set(MESSAGE_SEPARATOR "------------------------------------------------------")
MESSAGE(STATUS ${MESSAGE_SEPARATOR} ${MESSAGE_SEPARATOR})
MESSAGE(STATUS "set cross compile toolchain in ${CMAKE_HOST_SYSTEM_NAME} platform")

# 包含gcc头文件路径
SET(SYSTEM_PATH "-ID:/Programs2/w64devkit/x86_64-w64-mingw32/include")
set(CROSS_COMPILE_PREFIX D:/Programs2/w64devkit/bin/)

IF(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
    SET(CMAKE_C_COMPILER "${CROSS_COMPILE_PREFIX}gcc.exe")
    SET(CMAKE_CXX_COMPILER "${CROSS_COMPILE_PREFIX}g++.exe")
    SET(AS "${CROSS_COMPILE_PREFIX}as.exe")
    SET(AR "${CROSS_COMPILE_PREFIX}ar.exe")
    SET(OBJCOPY "${CROSS_COMPILE_PREFIX}objcopy.exe")
    SET(OBJDUMP "${CROSS_COMPILE_PREFIX}objdump.exe")
    SET(SIZE "${CROSS_COMPILE_PREFIX}size.exe")
ELSE()
    SET(CMAKE_C_COMPILER "${CROSS_COMPILE_PREFIX}gcc")
    SET(CMAKE_CXX_COMPILER "${CROSS_COMPILE_PREFIX}g++")
    SET(AS "${CROSS_COMPILE_PREFIX}as")
    SET(AR "${CROSS_COMPILE_PREFIX}ar")
    SET(OBJCOPY "${CROSS_COMPILE_PREFIX}objcopy")
    SET(OBJDUMP "${CROSS_COMPILE_PREFIX}objdump")
    SET(SIZE "${CROSS_COMPILE_PREFIX}size")
ENDIF(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")

# 使用的 C 语言版本；
SET(CMAKE_C_STANDARD 11)

# 使用的 cpp 版本；
SET(CMAKE_CXX_STANDARD 17)

# 生成 compile_commands.json，可配合 clangd 实现精准的代码关联与跳转；
SET(CMAKE_EXPORT_COMPILE_COMMANDS True)

# 彩色日志输出；
SET(CMAKE_COLOR_DIAGNOSTICS True)

# this makes the test compiles use static library option so that we don't need to pre-set linker flags and scripts
SET(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# 定义通用编译器参数; ${MCPU_FLAGS} 处理器内核信息 ${VFP_FLAGS} 浮点运算单元类型 ${SYSTEM_PATH} 编译器头文件路径
SET(CFCOMMON
    "${SYSTEM_PATH} -Wall"
)

# 设置默认构建类型为Debug
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# 定义最快运行速度发行模式的编译参数;
SET(CMAKE_C_FLAGS_RELEASE "-Os  ${CFCOMMON}")
SET(CMAKE_CXX_FLAGS_RELEASE "-Os  ${CFCOMMON} -fno-exceptions")
SET(CMAKE_ASM_FLAGS_RELEASE "${MCPU_FLAGS} ${VFP_FLAGS} -x assembler-with-cpp")

# 定义最小尺寸且包含调试信息的编译参数;
SET(CMAKE_C_FLAGS_RELWITHDEBINFO "-Os -g  ${CFCOMMON}")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-Os -g  ${CFCOMMON} -fno-exceptions")
SET(CMAKE_ASM_FLAGS_RELWITHDEBINFO "${MCPU_FLAGS} ${VFP_FLAGS} -x assembler-with-cpp")

# 定义最小尺寸的编译参数;
SET(CMAKE_C_FLAGS_MINSIZEREL "-Os  ${CFCOMMON}")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "-Os  ${CFCOMMON} -fno-exceptions")
SET(CMAKE_ASM_FLAGS_MINSIZEREL "${MCPU_FLAGS} ${VFP_FLAGS} -x assembler-with-cpp")

# 定义调试模式编译参数;
SET(CMAKE_C_FLAGS_DEBUG "-O0 -g  ${CFCOMMON}")
SET(CMAKE_CXX_FLAGS_DEBUG "-O0 -g  ${CFCOMMON} -fno-exceptions")
SET(CMAKE_ASM_FLAGS_DEBUG "${MCPU_FLAGS} ${VFP_FLAGS} -x assembler-with-cpp")

IF("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    MESSAGE(STATUS "Maximum optimization for speed")
ELSEIF("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    MESSAGE(STATUS "Maximum optimization for size, debug info included")
ELSEIF("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    MESSAGE(STATUS "Maximum optimization for size")
ELSE() # "Debug"
    MESSAGE(STATUS "No optimization : debug info included")
ENDIF()
MESSAGE(STATUS ${MESSAGE_SEPARATOR} ${MESSAGE_SEPARATOR} "\n")
