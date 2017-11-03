CXXFLAGS += -std=c++11 -W -Wall -g

# Set constant directorys
SDIR=src
INC=-Iinclude
# Preprocessor flags, MMD creates dependency files
CPPFLAGS += -MMD $(INC)

# List of source files
_SRCS=regex_engine.cpp penny_machine.cpp
SRCS=$(patsubst %,$(SDIR)/%,$(_SRCS))
	
bin/regex_engine: $(SRCS:%.cpp=%.o)
	mkdir -p bin
	g++ $(CPPFLAGS) $(CXXFLAGS) -o $@ $^

regex_engine_ref: regex_engine_ref.cpp
	g++ $(CXXFLAGS) -o $@ $^

clean:
	rm -f bin/*
	rm -f regex_engine_ref

.PHONY: clean

-include $(_SRCS:%.cpp=%.d)
