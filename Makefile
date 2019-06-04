.PHONY: all clean

CXX := g++
CXXFLAGS := \
    -std=c++14 \
    -Wall -Wextra \
    -Wconditionally-supported \
    -Wconversion \
    -Wduplicated-cond \
    -Wduplicated-branches \
    -Wextra-semi \
    -Wfloat-equal \
    -Wformat=2 \
    -Wlogical-op \
    -Wnull-dereference \
    -Wold-style-cast \
    -Wshadow \
    -Wswitch-default \
    -Wswitch-enum \
    -Wundef \
    -Wuseless-cast \
    -Wvla \
    -Wzero-as-null-pointer-constant
LDFLAGS :=

ifdef NO_OPTIMIZE
    CXXFLAGS += -g -fsanitize=undefined -fno-sanitize-recover=all -D_GLIBCXX_DEBUG
else
    CXXFLAGS += -O2 -march=native -mtune=native
endif

SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:%.cpp=%.o)
DEPS := $(SRCS:%.cpp=%.d)

TARGET := a.out

all: $(TARGET)

-include $(DEPS)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS) $(LDFLAGS)

test: $(TARGET)
	./$(TARGET) -test

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -MMD -MP $<

clean:
	-$(RM) $(TARGET) *.o
