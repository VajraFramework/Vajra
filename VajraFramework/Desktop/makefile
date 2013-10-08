################################################################################

WORKDIR = `pwd`

define all-cpp-files-under
$(subst ..,..,\
$(patsubst ./%, %, \
  $(shell cd $(WORKDIR) ; \
          find $(1) -name "*.cpp" -and -not -name ".*") \
 )\
)
endef

CXX = g++

LOCAL_SRC_DIRS = . ../Vajra
LOCAL_INCLUDES = . .. /usr/include ../Libraries

INC = $(addprefix -I, $(LOCAL_INCLUDES))
LOCAL_CFLAGS = -Wall -g -DPLATFORM_DESKTOP
LIBDIR = -L/usr/lib
LIB_FLAGS = -lglfw -lGL -lGLU -lGLEW -lpthread -lXxf86vm
LDFLAGS = 

BUILDDIR = build
OBJDIR = $(BUILDDIR)/obj
BINDIR = $(BUILDDIR)/bin
EXECUTABLE = $(BINDIR)/executable
RESOURCESDIR_NAME = res

# OBJECTS = $(OBJDIR)/desktopWrapper.o $(OBJDIR)/shader.o
# OBJECTS := $(patsubst %.cpp,$(OBJDIR)/%.o, $(call all-cpp-files-under, .))
OBJECTS += $(addprefix $(OBJDIR)/,\
				$(patsubst %.cpp,%.o,\
					$(call all-cpp-files-under, $(LOCAL_SRC_DIRS))\
				 )\
			)

all: build_all

run: all
	@echo "\nRUNNING EXECUTABLE";
	cd $(BINDIR)
	./$(EXECUTABLE)

before_build: 
	@echo "\n--------------------------------------------------------------------------------"
	@echo "INITIALIZING BUILD ENVIRONMENT."
	@test -d $(BUILDDIR) || mkdir -p $(BUILDDIR)
	@test -d $(OBJDIR) || mkdir -p $(OBJDIR)
	@test -d $(BINDIR) || mkdir -p $(BINDIR)
	@echo "\n--------------------------------------------------------------------------------"
	@echo "BUILDING OBJ FILES."
	@echo "List of all objs: $(OBJECTS) \n"
	@mkdir -p $(subst ..,__, $(dir $(OBJECTS)))

build_all: before_build build_executable after_build
	@echo "\n--------------------------------------------------------------------------------"
	@echo "\nBUILD SUCCEEDED."
	@echo "\n--------------------------------------------------------------------------------"

build_executable: before_build $(OBJECTS)
	@echo "\n--------------------------------------------------------------------------------"
	@echo "BUILDING EXECUTABLE.";
	$(CXX) $(LIBDIR) -o $(EXECUTABLE) $(subst ..,__, $(OBJECTS))  $(LDFLAGS) $(LIB_FLAGS)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(LOCAL_CFLAGS) $(INC) -c $^ -o $(subst ..,__, $@)

after_build: 
	@echo "\n--------------------------------------------------------------------------------"
	@echo "COPYING RESOURCES TO EXECUTABLE DIRECTORY."
	rsync -r -v --delete $(RESOURCESDIR_NAME) $(BINDIR)/.


clean:
	@echo "\n--------------------------------------------------------------------------------"
	@echo "CLEANING PROJECT.\n"
	-rm -rf $(BUILDDIR)

.PHONY: before_build after_build clean_debug

################################################################################
