# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM

# Include any dependencies generated for this target.
include src/ServerCore/CMakeFiles/ServerCore.dir/depend.make

# Include the progress variables for this target.
include src/ServerCore/CMakeFiles/ServerCore.dir/progress.make

# Include the compile flags for this target's objects.
include src/ServerCore/CMakeFiles/ServerCore.dir/flags.make

src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/Acceptor.cpp.o: src/ServerCore/CMakeFiles/ServerCore.dir/flags.make
src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/Acceptor.cpp.o: src/ServerCore/src/Network/Acceptor.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/Acceptor.cpp.o"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ServerCore.dir/src/Network/Acceptor.cpp.o -c /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Network/Acceptor.cpp

src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/Acceptor.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ServerCore.dir/src/Network/Acceptor.cpp.i"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Network/Acceptor.cpp > CMakeFiles/ServerCore.dir/src/Network/Acceptor.cpp.i

src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/Acceptor.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ServerCore.dir/src/Network/Acceptor.cpp.s"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Network/Acceptor.cpp -o CMakeFiles/ServerCore.dir/src/Network/Acceptor.cpp.s

src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/Service.cpp.o: src/ServerCore/CMakeFiles/ServerCore.dir/flags.make
src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/Service.cpp.o: src/ServerCore/src/Network/Service.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/Service.cpp.o"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ServerCore.dir/src/Network/Service.cpp.o -c /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Network/Service.cpp

src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/Service.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ServerCore.dir/src/Network/Service.cpp.i"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Network/Service.cpp > CMakeFiles/ServerCore.dir/src/Network/Service.cpp.i

src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/Service.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ServerCore.dir/src/Network/Service.cpp.s"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Network/Service.cpp -o CMakeFiles/ServerCore.dir/src/Network/Service.cpp.s

src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/Session.cpp.o: src/ServerCore/CMakeFiles/ServerCore.dir/flags.make
src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/Session.cpp.o: src/ServerCore/src/Network/Session.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/Session.cpp.o"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ServerCore.dir/src/Network/Session.cpp.o -c /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Network/Session.cpp

src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/Session.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ServerCore.dir/src/Network/Session.cpp.i"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Network/Session.cpp > CMakeFiles/ServerCore.dir/src/Network/Session.cpp.i

src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/Session.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ServerCore.dir/src/Network/Session.cpp.s"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Network/Session.cpp -o CMakeFiles/ServerCore.dir/src/Network/Session.cpp.s

src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/RecvBuffer.cpp.o: src/ServerCore/CMakeFiles/ServerCore.dir/flags.make
src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/RecvBuffer.cpp.o: src/ServerCore/src/Network/RecvBuffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/RecvBuffer.cpp.o"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ServerCore.dir/src/Network/RecvBuffer.cpp.o -c /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Network/RecvBuffer.cpp

src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/RecvBuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ServerCore.dir/src/Network/RecvBuffer.cpp.i"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Network/RecvBuffer.cpp > CMakeFiles/ServerCore.dir/src/Network/RecvBuffer.cpp.i

src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/RecvBuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ServerCore.dir/src/Network/RecvBuffer.cpp.s"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Network/RecvBuffer.cpp -o CMakeFiles/ServerCore.dir/src/Network/RecvBuffer.cpp.s

src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/SendBuffer.cpp.o: src/ServerCore/CMakeFiles/ServerCore.dir/flags.make
src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/SendBuffer.cpp.o: src/ServerCore/src/Network/SendBuffer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/SendBuffer.cpp.o"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ServerCore.dir/src/Network/SendBuffer.cpp.o -c /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Network/SendBuffer.cpp

src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/SendBuffer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ServerCore.dir/src/Network/SendBuffer.cpp.i"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Network/SendBuffer.cpp > CMakeFiles/ServerCore.dir/src/Network/SendBuffer.cpp.i

src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/SendBuffer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ServerCore.dir/src/Network/SendBuffer.cpp.s"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Network/SendBuffer.cpp -o CMakeFiles/ServerCore.dir/src/Network/SendBuffer.cpp.s

src/ServerCore/CMakeFiles/ServerCore.dir/src/Thread/ThreadManager.cpp.o: src/ServerCore/CMakeFiles/ServerCore.dir/flags.make
src/ServerCore/CMakeFiles/ServerCore.dir/src/Thread/ThreadManager.cpp.o: src/ServerCore/src/Thread/ThreadManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object src/ServerCore/CMakeFiles/ServerCore.dir/src/Thread/ThreadManager.cpp.o"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ServerCore.dir/src/Thread/ThreadManager.cpp.o -c /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Thread/ThreadManager.cpp

src/ServerCore/CMakeFiles/ServerCore.dir/src/Thread/ThreadManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ServerCore.dir/src/Thread/ThreadManager.cpp.i"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Thread/ThreadManager.cpp > CMakeFiles/ServerCore.dir/src/Thread/ThreadManager.cpp.i

src/ServerCore/CMakeFiles/ServerCore.dir/src/Thread/ThreadManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ServerCore.dir/src/Thread/ThreadManager.cpp.s"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Thread/ThreadManager.cpp -o CMakeFiles/ServerCore.dir/src/Thread/ThreadManager.cpp.s

src/ServerCore/CMakeFiles/ServerCore.dir/src/Thread/TLS.cpp.o: src/ServerCore/CMakeFiles/ServerCore.dir/flags.make
src/ServerCore/CMakeFiles/ServerCore.dir/src/Thread/TLS.cpp.o: src/ServerCore/src/Thread/TLS.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object src/ServerCore/CMakeFiles/ServerCore.dir/src/Thread/TLS.cpp.o"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ServerCore.dir/src/Thread/TLS.cpp.o -c /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Thread/TLS.cpp

src/ServerCore/CMakeFiles/ServerCore.dir/src/Thread/TLS.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ServerCore.dir/src/Thread/TLS.cpp.i"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Thread/TLS.cpp > CMakeFiles/ServerCore.dir/src/Thread/TLS.cpp.i

src/ServerCore/CMakeFiles/ServerCore.dir/src/Thread/TLS.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ServerCore.dir/src/Thread/TLS.cpp.s"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Thread/TLS.cpp -o CMakeFiles/ServerCore.dir/src/Thread/TLS.cpp.s

src/ServerCore/CMakeFiles/ServerCore.dir/src/Main/Singleton.cpp.o: src/ServerCore/CMakeFiles/ServerCore.dir/flags.make
src/ServerCore/CMakeFiles/ServerCore.dir/src/Main/Singleton.cpp.o: src/ServerCore/src/Main/Singleton.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object src/ServerCore/CMakeFiles/ServerCore.dir/src/Main/Singleton.cpp.o"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ServerCore.dir/src/Main/Singleton.cpp.o -c /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Main/Singleton.cpp

src/ServerCore/CMakeFiles/ServerCore.dir/src/Main/Singleton.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ServerCore.dir/src/Main/Singleton.cpp.i"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Main/Singleton.cpp > CMakeFiles/ServerCore.dir/src/Main/Singleton.cpp.i

src/ServerCore/CMakeFiles/ServerCore.dir/src/Main/Singleton.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ServerCore.dir/src/Main/Singleton.cpp.s"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Main/Singleton.cpp -o CMakeFiles/ServerCore.dir/src/Main/Singleton.cpp.s

src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/LockQueue.cpp.o: src/ServerCore/CMakeFiles/ServerCore.dir/flags.make
src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/LockQueue.cpp.o: src/ServerCore/src/Job/LockQueue.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/LockQueue.cpp.o"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ServerCore.dir/src/Job/LockQueue.cpp.o -c /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Job/LockQueue.cpp

src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/LockQueue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ServerCore.dir/src/Job/LockQueue.cpp.i"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Job/LockQueue.cpp > CMakeFiles/ServerCore.dir/src/Job/LockQueue.cpp.i

src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/LockQueue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ServerCore.dir/src/Job/LockQueue.cpp.s"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Job/LockQueue.cpp -o CMakeFiles/ServerCore.dir/src/Job/LockQueue.cpp.s

src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/Job.cpp.o: src/ServerCore/CMakeFiles/ServerCore.dir/flags.make
src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/Job.cpp.o: src/ServerCore/src/Job/Job.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/Job.cpp.o"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ServerCore.dir/src/Job/Job.cpp.o -c /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Job/Job.cpp

src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/Job.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ServerCore.dir/src/Job/Job.cpp.i"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Job/Job.cpp > CMakeFiles/ServerCore.dir/src/Job/Job.cpp.i

src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/Job.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ServerCore.dir/src/Job/Job.cpp.s"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Job/Job.cpp -o CMakeFiles/ServerCore.dir/src/Job/Job.cpp.s

src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/JobTimer.cpp.o: src/ServerCore/CMakeFiles/ServerCore.dir/flags.make
src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/JobTimer.cpp.o: src/ServerCore/src/Job/JobTimer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/JobTimer.cpp.o"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ServerCore.dir/src/Job/JobTimer.cpp.o -c /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Job/JobTimer.cpp

src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/JobTimer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ServerCore.dir/src/Job/JobTimer.cpp.i"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Job/JobTimer.cpp > CMakeFiles/ServerCore.dir/src/Job/JobTimer.cpp.i

src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/JobTimer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ServerCore.dir/src/Job/JobTimer.cpp.s"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Job/JobTimer.cpp -o CMakeFiles/ServerCore.dir/src/Job/JobTimer.cpp.s

src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/GlobalQueue.cpp.o: src/ServerCore/CMakeFiles/ServerCore.dir/flags.make
src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/GlobalQueue.cpp.o: src/ServerCore/src/Job/GlobalQueue.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/GlobalQueue.cpp.o"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ServerCore.dir/src/Job/GlobalQueue.cpp.o -c /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Job/GlobalQueue.cpp

src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/GlobalQueue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ServerCore.dir/src/Job/GlobalQueue.cpp.i"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Job/GlobalQueue.cpp > CMakeFiles/ServerCore.dir/src/Job/GlobalQueue.cpp.i

src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/GlobalQueue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ServerCore.dir/src/Job/GlobalQueue.cpp.s"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Job/GlobalQueue.cpp -o CMakeFiles/ServerCore.dir/src/Job/GlobalQueue.cpp.s

src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/JobQueue.cpp.o: src/ServerCore/CMakeFiles/ServerCore.dir/flags.make
src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/JobQueue.cpp.o: src/ServerCore/src/Job/JobQueue.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/JobQueue.cpp.o"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ServerCore.dir/src/Job/JobQueue.cpp.o -c /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Job/JobQueue.cpp

src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/JobQueue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ServerCore.dir/src/Job/JobQueue.cpp.i"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Job/JobQueue.cpp > CMakeFiles/ServerCore.dir/src/Job/JobQueue.cpp.i

src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/JobQueue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ServerCore.dir/src/Job/JobQueue.cpp.s"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Job/JobQueue.cpp -o CMakeFiles/ServerCore.dir/src/Job/JobQueue.cpp.s

src/ServerCore/CMakeFiles/ServerCore.dir/src/Util/LogManager.cpp.o: src/ServerCore/CMakeFiles/ServerCore.dir/flags.make
src/ServerCore/CMakeFiles/ServerCore.dir/src/Util/LogManager.cpp.o: src/ServerCore/src/Util/LogManager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Building CXX object src/ServerCore/CMakeFiles/ServerCore.dir/src/Util/LogManager.cpp.o"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ServerCore.dir/src/Util/LogManager.cpp.o -c /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Util/LogManager.cpp

src/ServerCore/CMakeFiles/ServerCore.dir/src/Util/LogManager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ServerCore.dir/src/Util/LogManager.cpp.i"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Util/LogManager.cpp > CMakeFiles/ServerCore.dir/src/Util/LogManager.cpp.i

src/ServerCore/CMakeFiles/ServerCore.dir/src/Util/LogManager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ServerCore.dir/src/Util/LogManager.cpp.s"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/src/Util/LogManager.cpp -o CMakeFiles/ServerCore.dir/src/Util/LogManager.cpp.s

# Object files for target ServerCore
ServerCore_OBJECTS = \
"CMakeFiles/ServerCore.dir/src/Network/Acceptor.cpp.o" \
"CMakeFiles/ServerCore.dir/src/Network/Service.cpp.o" \
"CMakeFiles/ServerCore.dir/src/Network/Session.cpp.o" \
"CMakeFiles/ServerCore.dir/src/Network/RecvBuffer.cpp.o" \
"CMakeFiles/ServerCore.dir/src/Network/SendBuffer.cpp.o" \
"CMakeFiles/ServerCore.dir/src/Thread/ThreadManager.cpp.o" \
"CMakeFiles/ServerCore.dir/src/Thread/TLS.cpp.o" \
"CMakeFiles/ServerCore.dir/src/Main/Singleton.cpp.o" \
"CMakeFiles/ServerCore.dir/src/Job/LockQueue.cpp.o" \
"CMakeFiles/ServerCore.dir/src/Job/Job.cpp.o" \
"CMakeFiles/ServerCore.dir/src/Job/JobTimer.cpp.o" \
"CMakeFiles/ServerCore.dir/src/Job/GlobalQueue.cpp.o" \
"CMakeFiles/ServerCore.dir/src/Job/JobQueue.cpp.o" \
"CMakeFiles/ServerCore.dir/src/Util/LogManager.cpp.o"

# External object files for target ServerCore
ServerCore_EXTERNAL_OBJECTS =

src/ServerCore/libServerCore.a: src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/Acceptor.cpp.o
src/ServerCore/libServerCore.a: src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/Service.cpp.o
src/ServerCore/libServerCore.a: src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/Session.cpp.o
src/ServerCore/libServerCore.a: src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/RecvBuffer.cpp.o
src/ServerCore/libServerCore.a: src/ServerCore/CMakeFiles/ServerCore.dir/src/Network/SendBuffer.cpp.o
src/ServerCore/libServerCore.a: src/ServerCore/CMakeFiles/ServerCore.dir/src/Thread/ThreadManager.cpp.o
src/ServerCore/libServerCore.a: src/ServerCore/CMakeFiles/ServerCore.dir/src/Thread/TLS.cpp.o
src/ServerCore/libServerCore.a: src/ServerCore/CMakeFiles/ServerCore.dir/src/Main/Singleton.cpp.o
src/ServerCore/libServerCore.a: src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/LockQueue.cpp.o
src/ServerCore/libServerCore.a: src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/Job.cpp.o
src/ServerCore/libServerCore.a: src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/JobTimer.cpp.o
src/ServerCore/libServerCore.a: src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/GlobalQueue.cpp.o
src/ServerCore/libServerCore.a: src/ServerCore/CMakeFiles/ServerCore.dir/src/Job/JobQueue.cpp.o
src/ServerCore/libServerCore.a: src/ServerCore/CMakeFiles/ServerCore.dir/src/Util/LogManager.cpp.o
src/ServerCore/libServerCore.a: src/ServerCore/CMakeFiles/ServerCore.dir/build.make
src/ServerCore/libServerCore.a: src/ServerCore/CMakeFiles/ServerCore.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/CMakeFiles --progress-num=$(CMAKE_PROGRESS_15) "Linking CXX static library libServerCore.a"
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && $(CMAKE_COMMAND) -P CMakeFiles/ServerCore.dir/cmake_clean_target.cmake
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ServerCore.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/ServerCore/CMakeFiles/ServerCore.dir/build: src/ServerCore/libServerCore.a

.PHONY : src/ServerCore/CMakeFiles/ServerCore.dir/build

src/ServerCore/CMakeFiles/ServerCore.dir/clean:
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore && $(CMAKE_COMMAND) -P CMakeFiles/ServerCore.dir/cmake_clean.cmake
.PHONY : src/ServerCore/CMakeFiles/ServerCore.dir/clean

src/ServerCore/CMakeFiles/ServerCore.dir/depend:
	cd /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore /home/lhg/arzMETA-GAME-SERVER-BOOST-MANY_ROOM/src/ServerCore/CMakeFiles/ServerCore.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/ServerCore/CMakeFiles/ServerCore.dir/depend

