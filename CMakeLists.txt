cmake_minimum_required(VERSION 2.8)
project(PID_GENERATION)
set (CMAKE_CXX_FLAGS "-fPIC -g -gdwarf-3 -std=c++11")
set (SOURCES
	Links.cpp
	Neuron.cpp
	DataRetrieval.cpp
	ParticleInformation.cpp
	SumPIDCalculation.cpp
	NeuronLayers.cpp
	)

add_library(pid_lib SHARED ${SOURCES})
add_executable(pid_gen RunProgram.cpp)
target_link_libraries(pid_gen pid_lib) 
