CC = gcc
CXX = g++
AR = ar

ifeq ($(debug), y)
	CPPFLAGS := -g -D_ASSERT_DEBUG_
else
	CPPFLAGS := -O3
endif

CPPFLAGS := $(CPPFLAGS) -fPIC -std=c++0x -DWRITE_TO_STAND -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H

INCLUDE = -I ./deps/include

LIBS = -L ./deps/lib -lgit2 \
	   -lcurl -lhttp_parser -lssl -lz

TEST = test
ADD = add
CMI = commit
RESET = reset
CHECK = check
PUSH = push

TEST_FILE = test.cpp
ADD_FILE = add.cpp
CMI_FILE = commit.cpp
RESET_FILE = reset.cpp
CHECK_FILE = checkout.cpp
PUSH_FILE = push.cpp

TEST_OBJS := $(patsubst %.cpp, %.o, $(TEST_FILE))
ADD_OBJS := $(patsubst %.cpp, %.o, $(ADD_FILE))
CMI_OBJS := $(patsubst %.cpp, %.o, $(CMI_FILE))
RESET_OBJS := $(patsubst %.cpp, %.o, $(RESET_FILE))
CHECK_OBJS := $(patsubst %.cpp, %.o, $(CHECK_FILE))
PUSH_OBJS := $(patsubst %.cpp, %.o, $(PUSH_FILE))

all : $(TEST) $(ADD) $(CMI) $(RESET) $(CHECK) $(PUSH)

$(TEST) : $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@ $(LIBS)
	rm *.o

$(ADD) : $(ADD_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@ $(LIBS)
	rm *.o

$(CMI) : $(CMI_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@ $(LIBS)
	rm *.o

$(RESET) : $(RESET_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@ $(LIBS)
	rm *.o

$(CHECK) : $(CHECK_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@ $(LIBS)
	rm *.o

$(PUSH) : $(PUSH_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@ $(LIBS)
	rm *.o

.cpp.o:
	$(CXX) $(CPPFLAGS) $(INCLUDE) -c $< -o $@


clean:
	rm -f *.o $(TEST) $(ADD) $(CMI) $(RESET) $(CHECK) $(PUSH)
