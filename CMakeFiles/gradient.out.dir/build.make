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
CMAKE_SOURCE_DIR = /home/jayesh/Documents/Unfolding

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jayesh/Documents/Unfolding

# Include any dependencies generated for this target.
include CMakeFiles/gradient.out.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/gradient.out.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/gradient.out.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/gradient.out.dir/flags.make

CMakeFiles/gradient.out.dir/gradient.cpp.o: CMakeFiles/gradient.out.dir/flags.make
CMakeFiles/gradient.out.dir/gradient.cpp.o: gradient.cpp
CMakeFiles/gradient.out.dir/gradient.cpp.o: CMakeFiles/gradient.out.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jayesh/Documents/Unfolding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/gradient.out.dir/gradient.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/gradient.out.dir/gradient.cpp.o -MF CMakeFiles/gradient.out.dir/gradient.cpp.o.d -o CMakeFiles/gradient.out.dir/gradient.cpp.o -c /home/jayesh/Documents/Unfolding/gradient.cpp

CMakeFiles/gradient.out.dir/gradient.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/gradient.out.dir/gradient.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jayesh/Documents/Unfolding/gradient.cpp > CMakeFiles/gradient.out.dir/gradient.cpp.i

CMakeFiles/gradient.out.dir/gradient.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/gradient.out.dir/gradient.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jayesh/Documents/Unfolding/gradient.cpp -o CMakeFiles/gradient.out.dir/gradient.cpp.s

# Object files for target gradient.out
gradient_out_OBJECTS = \
"CMakeFiles/gradient.out.dir/gradient.cpp.o"

# External object files for target gradient.out
gradient_out_EXTERNAL_OBJECTS =

gradient.out: CMakeFiles/gradient.out.dir/gradient.cpp.o
gradient.out: CMakeFiles/gradient.out.dir/build.make
gradient.out: libCGAL_Qt5_moc_and_resources.a
gradient.out: /usr/lib/x86_64-linux-gnu/libgmpxx.so
gradient.out: /usr/lib/x86_64-linux-gnu/libmpfr.so
gradient.out: /usr/lib/x86_64-linux-gnu/libgmp.so
gradient.out: /home/jayesh/Qt/Tools/QtDesignStudio/qt5_design_studio_reduced_version/lib/libQt5OpenGL.so.5.15.5
gradient.out: /home/jayesh/Qt/Tools/QtDesignStudio/qt5_design_studio_reduced_version/lib/libQt5Svg.so.5.15.5
gradient.out: /home/jayesh/Qt/Tools/QtDesignStudio/qt5_design_studio_reduced_version/lib/libQt5Widgets.so.5.15.5
gradient.out: /home/jayesh/Qt/Tools/QtDesignStudio/qt5_design_studio_reduced_version/lib/libQt5Gui.so.5.15.5
gradient.out: /home/jayesh/Qt/Tools/QtDesignStudio/qt5_design_studio_reduced_version/lib/libQt5Core.so.5.15.5
gradient.out: CMakeFiles/gradient.out.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jayesh/Documents/Unfolding/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable gradient.out"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/gradient.out.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/gradient.out.dir/build: gradient.out
.PHONY : CMakeFiles/gradient.out.dir/build

CMakeFiles/gradient.out.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/gradient.out.dir/cmake_clean.cmake
.PHONY : CMakeFiles/gradient.out.dir/clean

CMakeFiles/gradient.out.dir/depend:
	cd /home/jayesh/Documents/Unfolding && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jayesh/Documents/Unfolding /home/jayesh/Documents/Unfolding /home/jayesh/Documents/Unfolding /home/jayesh/Documents/Unfolding /home/jayesh/Documents/Unfolding/CMakeFiles/gradient.out.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/gradient.out.dir/depend

