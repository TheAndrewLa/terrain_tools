# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 4.0

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "D:\Program Files\CMake\bin\cmake.exe"

# The command to remove a file.
RM = "D:\Program Files\CMake\bin\cmake.exe" -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\terrain_tools\Generator

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\terrain_tools\Generator\build\x86_windows_msys_pe_64bit-Debug

# Utility rule file for Generator_autogen_timestamp_deps.

# Include any custom commands dependencies for this target.
include CMakeFiles/Generator_autogen_timestamp_deps.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Generator_autogen_timestamp_deps.dir/progress.make

CMakeFiles/Generator_autogen_timestamp_deps.dir/codegen:
.PHONY : CMakeFiles/Generator_autogen_timestamp_deps.dir/codegen

Generator_autogen_timestamp_deps: CMakeFiles/Generator_autogen_timestamp_deps.dir/build.make
.PHONY : Generator_autogen_timestamp_deps

# Rule to build all files generated by this target.
CMakeFiles/Generator_autogen_timestamp_deps.dir/build: Generator_autogen_timestamp_deps
.PHONY : CMakeFiles/Generator_autogen_timestamp_deps.dir/build

CMakeFiles/Generator_autogen_timestamp_deps.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\Generator_autogen_timestamp_deps.dir\cmake_clean.cmake
.PHONY : CMakeFiles/Generator_autogen_timestamp_deps.dir/clean

CMakeFiles/Generator_autogen_timestamp_deps.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" D:\terrain_tools\Generator D:\terrain_tools\Generator D:\terrain_tools\Generator\build\x86_windows_msys_pe_64bit-Debug D:\terrain_tools\Generator\build\x86_windows_msys_pe_64bit-Debug D:\terrain_tools\Generator\build\x86_windows_msys_pe_64bit-Debug\CMakeFiles\Generator_autogen_timestamp_deps.dir\DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/Generator_autogen_timestamp_deps.dir/depend

