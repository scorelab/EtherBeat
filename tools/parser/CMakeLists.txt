cmake_minimum_required(VERSION 2.8.9)


project(ethereum_parser)

#Define project name
set(PROJECT_NAME "ethereum_parser")
#Bring the headers, such as Block.h into the project
include_directories(include)


set(IS_LIBRARY FALSE)
if(NOT ${IS_LIBRARY})
  #Set SOURCES with all cpp files in src directory
  file(GLOB SOURCES "src/*.cpp")

  add_executable(${PROJECT_NAME} ${SOURCES})
  target_link_libraries (${PROJECT_NAME} leveldb ${BASE_LIBS})
endif()

# Build shared library for python
# todo
if(${IS_LIBRARY})
  #Set SOURCES with all cpp files in src directory
  file(GLOB SOURCES "src/*.cpp" "wrapper/*.cpp")

  # Find default python libraries and interpreter
  find_package(PythonInterp REQUIRED)
  find_package(PythonLibs REQUIRED)
  FIND_PACKAGE(Boost COMPONENTS python3)

  include_directories(${Boost_INCLUDE_DIR} ${PYTHON_INCLUDE_DIRS})
  LINK_LIBRARIES(${Boost_LIBRARIES} ${PYTHON_LIBRARIES})

  # Build and link the module
  add_library(${PROJECT_NAME} SHARED ${SOURCES})
  target_link_libraries(${PROJECT_NAME} leveldb ${BASE_LIBS} ${Boost_LIBRARIES} ${PYTHON_LIBRARIES})
  add_dependencies(${PROJECT_NAME} Boost)

  # Tweaks the name of the library to match what Python expects
  set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX .so)
  set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")
endif()
