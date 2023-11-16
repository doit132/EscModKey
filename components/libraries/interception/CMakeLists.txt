SET(PATH_WORKSPACE_ROOT ${CMAKE_SOURCE_DIR}/../../../)

TARGET_SOURCES(
  ${PROJECT_NAME}
  PRIVATE # {{BEGIN_TARGET_SOURCES}}
          ${CMAKE_CURRENT_LIST_DIR}/src/utils.cpp
          # {{END_TARGET_SOURCES}}
)

# Add the Common/inc directory to the project include list
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR} ${CMAKE_CURRENT_LIST_DIR}/inc)

add_library(INTERCEPTION_LIB SHARED IMPORTED GLOBAL)
set_property(TARGET INTERCEPTION_LIB PROPERTY IMPORTED_IMPLIB_RELEASE "${CMAKE_CURRENT_LIST_DIR}/lib/interception.lib")
set_property(TARGET INTERCEPTION_LIB PROPERTY IMPORTED_LOCATION_RELEASE "${CMAKE_CURRENT_LIST_DIR}/lib/interception.dll")
set_property(TARGET INTERCEPTION_LIB PROPERTY IMPORTED_IMPLIB_DEBUG "${CMAKE_CURRENT_LIST_DIR}/lib/interception.lib")
set_property(TARGET INTERCEPTION_LIB PROPERTY IMPORTED_LOCATION_DEBUG "${CMAKE_CURRENT_LIST_DIR}/lib/interception.dll")

# find_library: 在指定目录下查找指定库，并把库的绝对路径存放到变量里，其第一个参数是变量名称，第二个参数是库名称，第三个参数是 HINTS，第 4 个参数是路径，其它用法可以参考 cmake 文档
# 使用 find_library 的好处是在执行 cmake … 时就会去查找库是否存在，这样可以提前发现错误，不用等到链接时
# find_library 只能找到静态库吗, 为什么只有 dll 的时候找不到, 是还需要设置什么吗?
find_library(INTERCEPTION_LIB interception ${CMAKE_CURRENT_LIST_DIR}/lib)

#  target_link_libraries: 把目标文件与库文件进行链接
target_link_libraries (${PROJECT_NAME} ${INTERCEPTION_LIB})
