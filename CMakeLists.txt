cmake_minimum_required(VERSION 3.5)

PROJECT(jump-a-jump)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/includes/)

aux_source_directory(./src DIR_SRCS)

aux_source_directory(./src/types DIR_SRCS)
aux_source_directory(./src/print DIR_SRCS)
aux_source_directory(./src/frames DIR_SRCS)
aux_source_directory(./src/frames/homepage DIR_SRCS)
aux_source_directory(./src/frames/gamepage DIR_SRCS)
aux_source_directory(./src/frames/header DIR_SRCS)
aux_source_directory(./src/input DIR_SRCS)
aux_source_directory(./src/game DIR_SRCS)
aux_source_directory(./src/utils DIR_SRCS)

ADD_EXECUTABLE(${PROJECT_NAME} ${DIR_SRCS})
