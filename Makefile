CXX=g++
AR=ar

CFLAGS=-g -Wall
CXXFLAGS=$(CFLAGS) -std=c++11
ARFLAGS=rcs

SRCDIR=./src
OBJDIR=./obj
LIBDIR=./lib

INCPATHS=-I../include
LIBS=

SRCS=LBXFile.cpp LBXPackedFile.cpp
OBJS=$(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

vpath %.cpp $(SRCDIR)

lib: $(LIBDIR)/liblbx.a

$(LIBDIR)/liblbx.a: $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del $(OBJDIR)/*.o
	del $(LIBDIR)/*.a