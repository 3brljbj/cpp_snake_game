# 컴파일러 지정
CXX = g++
# 컴파일 옵션: C++11 표준 사용, 모든 경고 표시
CXXFLAGS = -std=c++11 -Wall
# 링커 라이브러리: ncurses 라이브러리 링크
LDFLAGS = -lncurses

# --- ★★★ 여기에 GateManager.cpp 추가 ★★★ ---
# 컴파일할 소스 파일 목록
SRCS = main.cpp Game.cpp Map.cpp Snake.cpp ItemManager.cpp UIManager.cpp GateManager.cpp
# 소스 파일로부터 생성될 오브젝트 파일 목록
OBJS = $(SRCS:.cpp=.o)
# 최종 실행 파일 이름
TARGET = snake_game

# 최종 목표(all): 실행 파일 생성
all: $(TARGET)

# 실행 파일 생성 규칙
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# .cpp 파일을 .o 파일로 컴파일하는 규칙
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 빌드 중간 파일들 삭제
clean:
	rm -f $(OBJS) $(TARGET)