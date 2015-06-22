
#set(CMAKE_VERBOSE_MAKEFILE ON)

# our exe name
project (bsp-quer)

cmake_minimum_required (VERSION 2.8)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -g")

find_package (SDL REQUIRED)
find_package (SDL_image REQUIRED)

find_package (Threads REQUIRED)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_SYSTEM ON)
find_package (Boost COMPONENTS thread system regex filesystem date_time REQUIRED)

# source files. sure, there's a better way of doing this
set (SRC
  src/main.cc 
  src/engine.cc 
  src/room.cc 
  src/interface.cc 
  src/player.cc 
  src/world.cc 
  src/loader.cc 
  src/item.cc 
  src/description.cc 
  src/exit.cc 
  src/application.cc 
  src/save_menu.cc 
  src/character.cc 
  src/sdl/screen.cc 
  src/sdl/event.cc 
  src/sdl/surface.cc
)


link_libraries(
  ${Boost_LIBRARIES}
  ${SDL_LIBRARY}
  ${SDL_IMAGE_LIBRARIES}
)

add_executable(text-adventure ${SRC} )

add_custom_command(TARGET text-adventure POST_BUILD 
  COMMAND rm -f ${CMAKE_CURRENT_BINARY_DIR}/data COMMENT "removing stale data directory"
  COMMAND ln -sf ${CMAKE_SOURCE_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data COMMENT "linking data directory"
)
