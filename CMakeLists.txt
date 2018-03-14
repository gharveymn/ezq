CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
PROJECT(ezq)

SET(CMAKE_C_STANDARD 11)

SET(SOURCE_FILES
	   src/ezq.c src/headers/ezq.h
	   src/mtezq.c src/headers/mtezq.h
	   tester.c)
ADD_EXECUTABLE(tester ${SOURCE_FILES})