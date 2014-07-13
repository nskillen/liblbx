CXX=g++
LD=ld

CFLAGS=-g -Wall
CXXFLAGS=$(CFLAGS) -std=c++11
LDFLAGS=-g

SRCDIR=../src
OBJDIR=../obj

INCPATHS=-I../../include -I../src

SRCS=LBXFileTests.cpp $(SRCDIR)/LBXFile.cpp $(SRCDIR)/LBXPackedFile.cpp
OBJS=$(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))

tests: TestLibLBX.exe

TestLibLBX.exe: $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCPATHS) -c $< -o $@

clean:
	del $(OBJDIR)/*.o
	del $(LIBDIR)/*.a