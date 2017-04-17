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

TEST_FILE = test.cpp
ADD_FILE = add.cpp
CMI_FILE = commit.cpp

TEST_OBJS := $(patsubst %.cpp, %.o, $(TEST_FILE))
ADD_OBJS := $(patsubst %.cpp, %.o, $(ADD_FILE))
CMI_OBJS := $(patsubst %.cpp, %.o, $(CMI_FILE))

all : $(TEST) $(ADD) $(CMI)

$(TEST) : $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@ $(LIBS)
	rm *.o

$(ADD) : $(ADD_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@ $(LIBS)
	rm *.o

$(CMI) : $(CMI_OBJS)
	$(CXX) $(CXXFLAGS) $(INCLUDE) $^ -o $@ $(LIBS)
	rm *.o

.cpp.o:
	$(CXX) $(CPPFLAGS) $(INCLUDE) -c $< -o $@


clean:
	rm -f *.o $(TEST) $(ADD) $(CMI)
