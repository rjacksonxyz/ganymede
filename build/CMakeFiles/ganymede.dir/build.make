# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.31

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /opt/homebrew/bin/cmake

# The command to remove a file.
RM = /opt/homebrew/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/rjacksonxyz/workspace/ganymede

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/rjacksonxyz/workspace/ganymede/build

# Include any dependencies generated for this target.
include CMakeFiles/ganymede.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/ganymede.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/ganymede.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ganymede.dir/flags.make

CMakeFiles/ganymede.dir/codegen:
.PHONY : CMakeFiles/ganymede.dir/codegen

CMakeFiles/ganymede.dir/src/order/order.cpp.o: CMakeFiles/ganymede.dir/flags.make
CMakeFiles/ganymede.dir/src/order/order.cpp.o: /Users/rjacksonxyz/workspace/ganymede/src/order/order.cpp
CMakeFiles/ganymede.dir/src/order/order.cpp.o: CMakeFiles/ganymede.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/rjacksonxyz/workspace/ganymede/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ganymede.dir/src/order/order.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ganymede.dir/src/order/order.cpp.o -MF CMakeFiles/ganymede.dir/src/order/order.cpp.o.d -o CMakeFiles/ganymede.dir/src/order/order.cpp.o -c /Users/rjacksonxyz/workspace/ganymede/src/order/order.cpp

CMakeFiles/ganymede.dir/src/order/order.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ganymede.dir/src/order/order.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/rjacksonxyz/workspace/ganymede/src/order/order.cpp > CMakeFiles/ganymede.dir/src/order/order.cpp.i

CMakeFiles/ganymede.dir/src/order/order.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ganymede.dir/src/order/order.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/rjacksonxyz/workspace/ganymede/src/order/order.cpp -o CMakeFiles/ganymede.dir/src/order/order.cpp.s

CMakeFiles/ganymede.dir/src/orderbook/orderbook.cpp.o: CMakeFiles/ganymede.dir/flags.make
CMakeFiles/ganymede.dir/src/orderbook/orderbook.cpp.o: /Users/rjacksonxyz/workspace/ganymede/src/orderbook/orderbook.cpp
CMakeFiles/ganymede.dir/src/orderbook/orderbook.cpp.o: CMakeFiles/ganymede.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/rjacksonxyz/workspace/ganymede/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/ganymede.dir/src/orderbook/orderbook.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/ganymede.dir/src/orderbook/orderbook.cpp.o -MF CMakeFiles/ganymede.dir/src/orderbook/orderbook.cpp.o.d -o CMakeFiles/ganymede.dir/src/orderbook/orderbook.cpp.o -c /Users/rjacksonxyz/workspace/ganymede/src/orderbook/orderbook.cpp

CMakeFiles/ganymede.dir/src/orderbook/orderbook.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/ganymede.dir/src/orderbook/orderbook.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/rjacksonxyz/workspace/ganymede/src/orderbook/orderbook.cpp > CMakeFiles/ganymede.dir/src/orderbook/orderbook.cpp.i

CMakeFiles/ganymede.dir/src/orderbook/orderbook.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/ganymede.dir/src/orderbook/orderbook.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/rjacksonxyz/workspace/ganymede/src/orderbook/orderbook.cpp -o CMakeFiles/ganymede.dir/src/orderbook/orderbook.cpp.s

# Object files for target ganymede
ganymede_OBJECTS = \
"CMakeFiles/ganymede.dir/src/order/order.cpp.o" \
"CMakeFiles/ganymede.dir/src/orderbook/orderbook.cpp.o"

# External object files for target ganymede
ganymede_EXTERNAL_OBJECTS =

libganymede.a: CMakeFiles/ganymede.dir/src/order/order.cpp.o
libganymede.a: CMakeFiles/ganymede.dir/src/orderbook/orderbook.cpp.o
libganymede.a: CMakeFiles/ganymede.dir/build.make
libganymede.a: CMakeFiles/ganymede.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/rjacksonxyz/workspace/ganymede/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libganymede.a"
	$(CMAKE_COMMAND) -P CMakeFiles/ganymede.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ganymede.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ganymede.dir/build: libganymede.a
.PHONY : CMakeFiles/ganymede.dir/build

CMakeFiles/ganymede.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ganymede.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ganymede.dir/clean

CMakeFiles/ganymede.dir/depend:
	cd /Users/rjacksonxyz/workspace/ganymede/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/rjacksonxyz/workspace/ganymede /Users/rjacksonxyz/workspace/ganymede /Users/rjacksonxyz/workspace/ganymede/build /Users/rjacksonxyz/workspace/ganymede/build /Users/rjacksonxyz/workspace/ganymede/build/CMakeFiles/ganymede.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/ganymede.dir/depend

