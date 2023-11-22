cmake_minimum_required(VERSION 3.12)

set(PROJECT_NAME MyEngine)

#project(MyEngine)

add_subdirectory(MyEngineCore)
add_subdirectory(MyEngineEditor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT MyEngineEditor)