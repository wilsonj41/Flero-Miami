SRCDIR = ./src
OBJDIR = ./obj
BINDIR = ./bin
SODIR = ./so
INCDIR = ./include
DBDIR = ../docker/public-html/driver
DB_INCDIR = /usr/include/mysql

CXX=g++
CXXWARN=-Wall -Wextra -Wshadow -Wpedantic -Wconversion -Werror\
 			-Wtrampolines -Wvla -Wlogical-op -Wduplicated-cond\
  			-Wduplicated-branches -Wformat-signedness -Wswitch-default -Wswitch-enum -Wno-stringop-overflow
CXXFLAGS=-std=c++20 $(CXXWARN) -O3 -I $(INCDIR) -fPIC -DFPM_BUILD
LDLIBS=-lpthread -lfcgi -lfcgi++ -lboost_filesystem -lboost_system -lboost_program_options -lcgicc -lspdlog -lfmt 
LDFLAGS=-rdynamic
BOOSTFLAGS=-DBOOST_BIND_GLOBAL_PLACEHOLDERS
DB_LDLIBS=-lsoci_core -lsoci_mysql -lmysqlclient

BIN=fleropp_fpm
SO=libfleropp.so
SRC=$(wildcard $(SRCDIR)/*.cpp)
OBJ=$(addprefix $(OBJDIR)/,$(notdir $(SRC:.cpp=.o)))
GCH=$(INCDIR)/HTMLLiterals.hpp
MYSQL_DB_SRC=$(SRCDIR)/db_driver/mysql.cpp
MYSQL_DB_SO=db_driver_mysql.so

ifeq ($(PREFIX),)
	PREFIX := /usr
endif

.PHONY : all
all: $(BINDIR) $(OBJDIR) $(SODIR) compile link gch shared dbdriver

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
	$(CXX) $(CXXFLAGS) -shared -o $(SODIR)/$(SO) $(OBJ) $(LDLIBS)

.PHONY : dbdriver
dbdriver:
	$(CXX) $(CXXFLAGS) -I $(DB_INCDIR) -shared -o $(DBDIR)/$(MYSQL_DB_SO) $(MYSQL_DB_SRC) $(LDLIBS) $(DB_LDLIBS)

.PHONY : clean
clean:
	find -type f -name "$(BIN)" -delete
	find -type f -name "*.o" -delete
	find -type f -name "*~" -delete

install: 
	install -d $(DESTDIR)$(PREFIX)/lib/fleropp/drivers
	install -d $(DESTDIR)$(PREFIX)/include/fleropp
	install -d $(DESTDIR)$(PREFIX)/bin
	install -m 755 $(SODIR)/$(SO) $(DESTDIR)$(PREFIX)/lib/fleropp
	install -m 755 $(DBDIR)/$(MYSQL_DB_SO) $(DESTDIR)$(PREFIX)/lib/fleropp/drivers
	cp -r $(INCDIR)/* $(DESTDIR)$(PREFIX)/include/fleropp
	chmod -R 644 $(DESTDIR)$(PREFIX)/include/fleropp/*
	install -m 755 $(BINDIR)/$(BIN) $(DESTDIR)$(PREFIX)/bin

