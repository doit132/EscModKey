
# main:
# 	g++ -o main.exe main.cpp -std=c++11
# 	./main.exe
# test:
# 	g++ -o test.out test.cpp  -L./x64 -linterception
# 	g++ -o utils.out utils.cpp -std=c++11 -L./x64 -linterception
# 	ld -o test.exe test.out utils.out
# 	./test.exe

# 编译器设置
CXX = g++
CXXFLAGS = -Wextra -std=c++11 -fexec-charset=gbk
LDFLAGS = -L. -linterception

# 源文件和目标文件列表
# SRCS = hardwareid.cpp utils.cpp
# SRCS = test.cpp utils.cpp
SRCS = EscModKey_interception.cpp utils.cpp
OBJS = $(SRCS:.cpp=.o)
EXECUTABLE = EscModKey_interception.exe

# 默认目标
all: $(EXECUTABLE)

# 链接目标
$(EXECUTABLE): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(EXECUTABLE) $(LDFLAGS)
	rm *.o
	# ./$(EXECUTABLE)

# 生成目标文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	del $(OBJS) $(EXECUTABLE)

