MODULES =   BasicPheromone
CC           = g++ 
CCFLAGS      = -Wall -O2
LDFLAGS      = `sdl-config --cflags --libs` -lSDL_image
RM           = rm -f 
MAKE         = make 
INCLUDE      = -Iinclude/ext -Iinclude/contrib -Iinclude/core -Iprj/
SRC_C	     = $(wildcard src/core/*.c src/contrib/*.c src/contrib/**/*.c src/ext/*.c)
SRC_CC	     = $(wildcard src/core/*.cpp src/contrib/*.cpp src/contrib/**/*.cpp src/ext/*.cpp)

# if DEBUG is enabled, add -g option to CCFLAGS
ifneq ($(strip $(DEBUG)),)
	CCFLAGS += -g
endif

# if MODULES is empty, compile everything
ifeq ($(strip $(MODULES)),)
	MODULES := ${shell ls -1 prj | grep '/' | cut -d'/' -f 1}
endif

# Preprocessor variables associated with the modules to be compiled
MOD_VARS := -DMODULAR $(foreach m, $(MODULES), -DPRJ_${shell echo $(m) | tr [a-z] [A-Z] | sed 's/\-/\_/g'})
CCFLAGS += $(MOD_VARS)

# Update SRC_C And SRC_CC with modules files 
SRC_C += $(foreach m, $(MODULES), $(wildcard prj/$(m)/src/*.c) $(wildcard prj/$(m)/src/**/*.c))
SRC_CC += $(foreach m, $(MODULES), $(wildcard prj/$(m)/src/*.cpp) $(wildcard prj/$(m)/src/**/*.cpp))

OBJ          = $(SRC_C:.c=.o) $(SRC_CC:.cpp=.o)
TARGET       := roborobo

.PHONY: snapshot

.IGNORE: 

all: verifyMods $(TARGET)

verifyMods:
	@for i in $(MODULES); do \
		if [ ! -d prj/$$i ]; then \
			echo "WARNING : Project $$i does not exist !"; \
		fi; \
	done


$(TARGET): $(OBJ)
ifeq ($(strip $(VERBOSE)),)
	@echo "[LD]	" $@
	@$(CC) -o $@ $^ $(LDFLAGS) 
else
	$(CC) -o $@ $^ $(LDFLAGS) 
endif

%.o: %.cpp
ifeq ($(strip $(VERBOSE)),)
	@echo "[CPP]	" $<
	@$(CC) -o $@ -c $< $(CCFLAGS) $(INCLUDE)
else
	$(CC) -o $@ -c $< $(CCFLAGS) $(INCLUDE)
endif

%.o: %.c
ifeq ($(strip $(VERBOSE)),)
	@echo "[C]	" $<
	@$(CC) -o $@ -c $< $(CCFLAGS) $(INCLUDE)
else
	$(CC) -o $@ -c $< $(CCFLAGS) $(INCLUDE)
endif

clean:
ifeq ($(strip $(VERBOSE)),)
	@$(RM) $(OBJ) 
else
	$(RM) $(OBJ) 
endif

distclean:
	@$(MAKE) clean 
	$(RM) $(TARGET) 

snapshot:
	clear
	make distclean
	rm -f roborobo-snapshot.tgz
	tar --exclude='._*' --exclude='build' --exclude='.DS_Store' --exclude='datalog_*' --exclude='properties_*' --exclude='roborobo-snapshot.tgz' --exclude='.svn' -cvf - ../`pwd | perl -ne '/.*\/(.*)$$/; print "$$1\n"';`/* | gzip -c > roborobo-snapshot.tgz
	@echo =-=-=-= 
	@echo =-=-=-=
	@echo =-=-=-=
	@echo Snapshot is created as roborobo-snapshot.tgz
	@echo .
	@echo All files have been archived except:
	@echo - datalog_* and properties_* files are NOT in the archive, wherever they were.
	@echo - .svn directories and contents are NOT in the archive
	@echo - macosx only: the macosx build directory is NOT in the archive
	@echo - macosx only: after uncompressing, re-create the symbolic link in the build/Debug dir.
	@echo - note: dont forget to re-compile as a make distclean was performed prior to building the archive
	@echo =-=-=-=
	@echo =-=-=-=
	@echo =-=-=-=

	

