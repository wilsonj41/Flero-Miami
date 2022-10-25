SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./bin
INCDIR = ./include

CXX=g++
CXXFLAGS=-std=c++20 -Wall -I $(INCDIR)
LDLIBS=-lfcgi -lfcgi++ -lboost_filesystem -lboost_system -lcgicc
LDFLAGS=-Wl,--start-group
BOOSTFLAGS=-DBOOST_BIND_GLOBAL_PLACEHOLDERS

BIN=fleropp_fpm
SRC=$(wildcard $(SRCDIR)/*.cpp)
OBJ=$(addprefix $(OBJDIR)/,$(notdir $(SRC:.cpp=.o)))
GCH=$(INCDIR)/HTMLLiterals.hpp

.PHONY : all
all: $(BINDIR) $(OBJDIR) compile link gch

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

.PHONY : compile
compile: $(OBJ)

.PHONY : gch
gch: $(GCH)
	$(CXX) $(CXXFLAGS) $<

.PHONY : $(OBJDIR)/%.o
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) $(BOOSTFLAGS) $< -o $@

.PHONY : link
link: compile
	$(CXX) $(CXXFLAGS) -o $(BINDIR)/$(BIN) $(OBJ) $(LDLIBS) 

.PHONY : clean
clean:
	find -type f -name "$(BIN)" -delete
	find -type f -name "*.o" -delete
	find -type f -name "*~" -delete

