# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ubuntu/repo-refactor

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ubuntu/repo-refactor/build2

# Include any dependencies generated for this target.
include lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/compiler_depend.make

# Include the progress variables for this target.
include lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/progress.make

# Include the compile flags for this target's objects.
include lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/flags.make

lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/src/op-attrs.cc.o: lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/flags.make
lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/src/op-attrs.cc.o: ../lib/op-attrs/ffi/src/op-attrs.cc
lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/src/op-attrs.cc.o: lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ubuntu/repo-refactor/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/src/op-attrs.cc.o"
	cd /home/ubuntu/repo-refactor/build2/lib/op-attrs/ffi && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/src/op-attrs.cc.o -MF CMakeFiles/op-attrs-ffi.dir/src/op-attrs.cc.o.d -o CMakeFiles/op-attrs-ffi.dir/src/op-attrs.cc.o -c /home/ubuntu/repo-refactor/lib/op-attrs/ffi/src/op-attrs.cc

lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/src/op-attrs.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/op-attrs-ffi.dir/src/op-attrs.cc.i"
	cd /home/ubuntu/repo-refactor/build2/lib/op-attrs/ffi && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ubuntu/repo-refactor/lib/op-attrs/ffi/src/op-attrs.cc > CMakeFiles/op-attrs-ffi.dir/src/op-attrs.cc.i

lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/src/op-attrs.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/op-attrs-ffi.dir/src/op-attrs.cc.s"
	cd /home/ubuntu/repo-refactor/build2/lib/op-attrs/ffi && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ubuntu/repo-refactor/lib/op-attrs/ffi/src/op-attrs.cc -o CMakeFiles/op-attrs-ffi.dir/src/op-attrs.cc.s

# Object files for target op-attrs-ffi
op__attrs__ffi_OBJECTS = \
"CMakeFiles/op-attrs-ffi.dir/src/op-attrs.cc.o"

# External object files for target op-attrs-ffi
op__attrs__ffi_EXTERNAL_OBJECTS =

lib/op-attrs/ffi/libop-attrs-ffi.so: lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/src/op-attrs.cc.o
lib/op-attrs/ffi/libop-attrs-ffi.so: lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/build.make
lib/op-attrs/ffi/libop-attrs-ffi.so: lib/utils/ffi/libutils-ffi.so
lib/op-attrs/ffi/libop-attrs-ffi.so: lib/op-attrs/libop-attrs.so
lib/op-attrs/ffi/libop-attrs-ffi.so: lib/utils/libutils.so
lib/op-attrs/ffi/libop-attrs-ffi.so: deps/fmt/libfmtd.a
lib/op-attrs/ffi/libop-attrs-ffi.so: deps/rapidcheck/librapidcheck.a
lib/op-attrs/ffi/libop-attrs-ffi.so: /usr/local/cuda/lib64/stubs/libcuda.so
lib/op-attrs/ffi/libop-attrs-ffi.so: /usr/local/cuda/lib64/libcudart_static.a
lib/op-attrs/ffi/libop-attrs-ffi.so: /usr/lib/x86_64-linux-gnu/librt.a
lib/op-attrs/ffi/libop-attrs-ffi.so: /usr/local/cuda/lib64/libcublas.so
lib/op-attrs/ffi/libop-attrs-ffi.so: /usr/local/cuda/lib64/libcurand.so
lib/op-attrs/ffi/libop-attrs-ffi.so: lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ubuntu/repo-refactor/build2/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX shared library libop-attrs-ffi.so"
	cd /home/ubuntu/repo-refactor/build2/lib/op-attrs/ffi && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/op-attrs-ffi.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/build: lib/op-attrs/ffi/libop-attrs-ffi.so
.PHONY : lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/build

lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/clean:
	cd /home/ubuntu/repo-refactor/build2/lib/op-attrs/ffi && $(CMAKE_COMMAND) -P CMakeFiles/op-attrs-ffi.dir/cmake_clean.cmake
.PHONY : lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/clean

lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/depend:
	cd /home/ubuntu/repo-refactor/build2 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ubuntu/repo-refactor /home/ubuntu/repo-refactor/lib/op-attrs/ffi /home/ubuntu/repo-refactor/build2 /home/ubuntu/repo-refactor/build2/lib/op-attrs/ffi /home/ubuntu/repo-refactor/build2/lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : lib/op-attrs/ffi/CMakeFiles/op-attrs-ffi.dir/depend

