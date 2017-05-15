
CC = gcc
CXX = g++
AR = ar

ifeq ($(debug), y)
	CPPFLAGS := -g -O2 -D_ASSERT_DEBUG_
else
	CPPFLAGS := -O3
endif

CPPFLAGS := $(CPPFLAGS) -fPIC -std=c++0x -DWRITE_TO_STAND -DHAVE_INTTYPES_H -DHAVE_NETINET_IN_H

INCLUDE = -I ./deps/include

LIBS = -L ./deps/lib -lgit2 \
	   -lcurl -lhttp_parser -lssl -lz

TARGET = libgittool.a

SRC_FILE = $(wildcard ./*.cpp)

OBJS := $(patsubst %.cpp, %.o, $(SRC_FILE))

.cpp.o:
	$(CXX) $(CPPFLAGS) $(INCLUDE) -c $< -o $@

$(TARGET) : $(OBJS)
	$(AR) rvs $@ $^
	rm *.o

clean:
	rm -f *.a *.o
