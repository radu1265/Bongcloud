CXXFLAGS = -g -Wall -std=c++17
LDLIBS =

PRGM  = Main
SRCS := $(wildcard *.cpp)
HDRS := $(wildcard *.h)
OBJSH := $(HDRS:.h=.o)
DEPSH := $(OBJSH:.o=.d)

OBJS := $(SRCS:.cpp=.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all clean
.SILENT: run

all: $(PRGM)

$(PRGM): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LDLIBS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

run: $(PRGM)
	./$(PRGM)

clean:
	rm -rf $(OBJS) $(OBJSH) $(DEPS) $(DEPSH)
	rm -rf $(PRGM)

xboard: 
	xboard -fcp "make run" -debug