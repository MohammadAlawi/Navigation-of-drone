# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/home/uwb5/git/ZED2/zed-examples/tutorials/tutorial 4 - positional tracking/cpp"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/home/uwb5/git/ZED2/zed-examples/tutorials/tutorial 4 - positional tracking/cpp/build"

# Include any dependencies generated for this target.
include CMakeFiles/ZED_Tutorial_4.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/ZED_Tutorial_4.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/ZED_Tutorial_4.dir/flags.make

CMakeFiles/ZED_Tutorial_4.dir/main.o: CMakeFiles/ZED_Tutorial_4.dir/flags.make
CMakeFiles/ZED_Tutorial_4.dir/main.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/home/uwb5/git/ZED2/zed-examples/tutorials/tutorial 4 - positional tracking/cpp/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/ZED_Tutorial_4.dir/main.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ZED_Tutorial_4.dir/main.o -c "/home/uwb5/git/ZED2/zed-examples/tutorials/tutorial 4 - positional tracking/cpp/main.cpp"

CMakeFiles/ZED_Tutorial_4.dir/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ZED_Tutorial_4.dir/main.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/home/uwb5/git/ZED2/zed-examples/tutorials/tutorial 4 - positional tracking/cpp/main.cpp" > CMakeFiles/ZED_Tutorial_4.dir/main.i

CMakeFiles/ZED_Tutorial_4.dir/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ZED_Tutorial_4.dir/main.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/home/uwb5/git/ZED2/zed-examples/tutorials/tutorial 4 - positional tracking/cpp/main.cpp" -o CMakeFiles/ZED_Tutorial_4.dir/main.s

CMakeFiles/ZED_Tutorial_4.dir/main.o.requires:

.PHONY : CMakeFiles/ZED_Tutorial_4.dir/main.o.requires

CMakeFiles/ZED_Tutorial_4.dir/main.o.provides: CMakeFiles/ZED_Tutorial_4.dir/main.o.requires
	$(MAKE) -f CMakeFiles/ZED_Tutorial_4.dir/build.make CMakeFiles/ZED_Tutorial_4.dir/main.o.provides.build
.PHONY : CMakeFiles/ZED_Tutorial_4.dir/main.o.provides

CMakeFiles/ZED_Tutorial_4.dir/main.o.provides.build: CMakeFiles/ZED_Tutorial_4.dir/main.o


# Object files for target ZED_Tutorial_4
ZED_Tutorial_4_OBJECTS = \
"CMakeFiles/ZED_Tutorial_4.dir/main.o"

# External object files for target ZED_Tutorial_4
ZED_Tutorial_4_EXTERNAL_OBJECTS =

ZED_Tutorial_4: CMakeFiles/ZED_Tutorial_4.dir/main.o
ZED_Tutorial_4: CMakeFiles/ZED_Tutorial_4.dir/build.make
ZED_Tutorial_4: /usr/local/zed/lib/libsl_zed.so
ZED_Tutorial_4: /usr/lib/aarch64-linux-gnu/libopenblas.so
ZED_Tutorial_4: /usr/lib/aarch64-linux-gnu/libusb-1.0.so
ZED_Tutorial_4: /usr/lib/aarch64-linux-gnu/libcuda.so
ZED_Tutorial_4: /usr/local/cuda/lib64/libcudart.so
ZED_Tutorial_4: CMakeFiles/ZED_Tutorial_4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/home/uwb5/git/ZED2/zed-examples/tutorials/tutorial 4 - positional tracking/cpp/build/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ZED_Tutorial_4"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ZED_Tutorial_4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/ZED_Tutorial_4.dir/build: ZED_Tutorial_4

.PHONY : CMakeFiles/ZED_Tutorial_4.dir/build

CMakeFiles/ZED_Tutorial_4.dir/requires: CMakeFiles/ZED_Tutorial_4.dir/main.o.requires

.PHONY : CMakeFiles/ZED_Tutorial_4.dir/requires

CMakeFiles/ZED_Tutorial_4.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/ZED_Tutorial_4.dir/cmake_clean.cmake
.PHONY : CMakeFiles/ZED_Tutorial_4.dir/clean

CMakeFiles/ZED_Tutorial_4.dir/depend:
	cd "/home/uwb5/git/ZED2/zed-examples/tutorials/tutorial 4 - positional tracking/cpp/build" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/home/uwb5/git/ZED2/zed-examples/tutorials/tutorial 4 - positional tracking/cpp" "/home/uwb5/git/ZED2/zed-examples/tutorials/tutorial 4 - positional tracking/cpp" "/home/uwb5/git/ZED2/zed-examples/tutorials/tutorial 4 - positional tracking/cpp/build" "/home/uwb5/git/ZED2/zed-examples/tutorials/tutorial 4 - positional tracking/cpp/build" "/home/uwb5/git/ZED2/zed-examples/tutorials/tutorial 4 - positional tracking/cpp/build/CMakeFiles/ZED_Tutorial_4.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/ZED_Tutorial_4.dir/depend

