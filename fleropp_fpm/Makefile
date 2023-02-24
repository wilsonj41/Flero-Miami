SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./bin
SODIR = ./so
INCDIR = ./include

CXX=g++
CXXWARN=-Wall -Wextra -Wshadow -Wpedantic -Wconversion -Werror\
 			-Wtrampolines -Wvla -Wlogical-op -Wduplicated-cond\
  			-Wduplicated-branches -Wformat-signedness -Wswitch-default -Wswitch-enum
CXXFLAGS=-std=c++20 $(CXXWARN) -O3 -I $(INCDIR) -fPIC -DFPM_BUILD
LDLIBS=-lpthread -lfcgi -lfcgi++ -lboost_filesystem -lboost_system -lboost_program_options -lcgicc -lspdlog -lfmt
LDFLAGS=-rdynamic
BOOSTFLAGS=-DBOOST_BIND_GLOBAL_PLACEHOLDERS

BIN=fleropp_fpm
SO=libfleropp.so
SRC=$(wildcard $(SRCDIR)/*.cpp)
OBJ=$(addprefix $(OBJDIR)/,$(notdir $(SRC:.cpp=.o)))
GCH=$(INCDIR)/HTMLLiterals.hpp

.PHONY : all
all: $(BINDIR) $(OBJDIR) $(SODIR) compile link gch shared

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(SODIR):
	mkdir -p $(SODIR)

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
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(BINDIR)/$(BIN) $(OBJ) $(LDLIBS)

.PHONY : shared
shared: compile
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -shared -o $(SODIR)/$(SO) $(OBJ) $(LDLIBS)

.PHONY : clean
clean:
	find -type f -name "$(BIN)" -delete
	find -type f -name "*.o" -delete
	find -type f -name "*~" -delete

