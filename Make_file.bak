
# 源文件和目标文件列表
# SRCS = hardwareid.cpp utils.cpp
# SRCS = test.cpp utils.cpp
BIN_DIR = bin
LIB_DIR = lib
INC_DIRS = include
SRCS = src/EscModKey_interception.cpp src/utils.cpp
OBJS = $(SRCS:.cpp=.o)
EXECUTABLE = EscModKey_interception.exe

# 编译器设置
CXX = g++
CXXFLAGS = -Wextra -std=c++11 -fexec-charset=gbk
LDFLAGS = -L$(LIB_DIR) -linterception

INCFLAGS := $(patsubst %, -I %, $(INC_DIRS))

CXXFLAGS += $(INCFLAGS)

# 默认目标
all: $(BIN_DIR)/$(EXECUTABLE)

# 链接目标
$(BIN_DIR)/$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(BIN_DIR)/$(EXECUTABLE) $(LDFLAGS)
	rm src/*.o

# 生成目标文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm src/*.o $(BIN_DIR)/$(EXECUTABLE)

