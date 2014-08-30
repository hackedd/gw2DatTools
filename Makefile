PROJECT = gw2dattools
LIBRARY = lib$(PROJECT).a
CC = g++

INCLUDE = -I./include
COMPILE_OPTIONS = $(INCLUDE) -g -std=c++11 -pedantic -Wall
DEPENDENCY_OPTIONS = $(INCLUDE) -std=c++11 -MM

SRC := src/gw2DatTools
DIRS := c_api compression exception format interface utils
SOURCE_FILES := $(foreach d, $(DIRS), $(wildcard $(SRC)/$(d)/*.cpp) )
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCE_FILES))
DEPENDENCIES = $(patsubst %.cpp, %.d, $(SOURCE_FILES))

EXTRACTOR_SOURCE_FILES := examples/src/simpleExtractor.cpp
EXTRACTOR_OBJECTS = $(patsubst %.cpp, %.o, $(EXTRACTOR_SOURCE_FILES))
EXTRACTOR_DEPENDENCIES = $(patsubst %.cpp, %.d, $(EXTRACTOR_SOURCE_FILES))

all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	ar -cr $(LIBRARY) $(OBJECTS) $(LIBS)

unpack-dat: $(LIBRARY) $(EXTRACTOR_OBJECTS)
	$(CC) -o unpack-dat $(EXTRACTOR_OBJECTS) $(LIBRARY)

.PHONY: clean
clean:
	rm -f $(LIBRARY) $(OBJECTS) unpack-dat $(EXTRACTOR_OBJECTS)

.PHONY: depclean
depclean:
	rm -f $(DEPENDENCIES) $(EXTRACTOR_DEPENDENCIES)

%.d: %.cpp
	$(CC) $(DEPENDENCY_OPTIONS) $< -MT "$*.o $*.d" -MF $*.d

%.o: %.cpp
	$(CC) -c $(COMPILE_OPTIONS) -o $@ $<

-include $(DEPENDENCIES)
-include $(EXTRACTOR_DEPENDENCIES)
