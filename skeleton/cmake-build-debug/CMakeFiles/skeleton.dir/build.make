# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/student/Downloads/clion-2019.2.5/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/student/Downloads/clion-2019.2.5/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/student/Documents/OSProjects/smash_skeleton/skeleton

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/student/Documents/OSProjects/smash_skeleton/skeleton/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/skeleton.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/skeleton.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/skeleton.dir/flags.make

CMakeFiles/skeleton.dir/smash.cpp.o: CMakeFiles/skeleton.dir/flags.make
CMakeFiles/skeleton.dir/smash.cpp.o: ../smash.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/student/Documents/OSProjects/smash_skeleton/skeleton/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/skeleton.dir/smash.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/skeleton.dir/smash.cpp.o -c /home/student/Documents/OSProjects/smash_skeleton/skeleton/smash.cpp

CMakeFiles/skeleton.dir/smash.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/skeleton.dir/smash.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/student/Documents/OSProjects/smash_skeleton/skeleton/smash.cpp > CMakeFiles/skeleton.dir/smash.cpp.i

CMakeFiles/skeleton.dir/smash.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/skeleton.dir/smash.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/student/Documents/OSProjects/smash_skeleton/skeleton/smash.cpp -o CMakeFiles/skeleton.dir/smash.cpp.s

# Object files for target skeleton
skeleton_OBJECTS = \
"CMakeFiles/skeleton.dir/smash.cpp.o"

# External object files for target skeleton
skeleton_EXTERNAL_OBJECTS =

skeleton: CMakeFiles/skeleton.dir/smash.cpp.o
skeleton: CMakeFiles/skeleton.dir/build.make
skeleton: CMakeFiles/skeleton.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/student/Documents/OSProjects/smash_skeleton/skeleton/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable skeleton"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/skeleton.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/skeleton.dir/build: skeleton

.PHONY : CMakeFiles/skeleton.dir/build

CMakeFiles/skeleton.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/skeleton.dir/cmake_clean.cmake
.PHONY : CMakeFiles/skeleton.dir/clean

CMakeFiles/skeleton.dir/depend:
	cd /home/student/Documents/OSProjects/smash_skeleton/skeleton/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/student/Documents/OSProjects/smash_skeleton/skeleton /home/student/Documents/OSProjects/smash_skeleton/skeleton /home/student/Documents/OSProjects/smash_skeleton/skeleton/cmake-build-debug /home/student/Documents/OSProjects/smash_skeleton/skeleton/cmake-build-debug /home/student/Documents/OSProjects/smash_skeleton/skeleton/cmake-build-debug/CMakeFiles/skeleton.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/skeleton.dir/depend

