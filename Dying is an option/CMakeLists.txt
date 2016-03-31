project(DIAO)
cmake_minimum_required(VERSION 2.8)

if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()


SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
SET(STATIC_SFML FALSE CACHE BOOL "Choose whether SFML is linked statically or not.")
SET(STATIC_RUNTIME FALSE CACHE BOOL "Use statically linked standard/runtime libraries? This option must match the one used for SFML.")

if(CMAKE_COMPILER_IS_GNUCXX)
  if(XY_STATIC_RUNTIME)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y -static")
  else()
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y")
  endif()
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++1y")
endif()


SET(CMAKE_CXX_FLAGS_DEBUG "-g -D_DEBUG_")
SET(CMAKE_CXX_FLAGS_RELEASE "-O4 -DNDEBUG")


if(STATIC_SFML)
  SET(SFML_STATIC_LIBRARIES TRUE)
endif()

if(WIN32)
  find_package(SFML 2 REQUIRED graphics window system audio main)
else()
  find_package(SFML 2 REQUIRED graphics window system audio)
endif()

if(UNIX)
  find_package(X11 REQUIRED)
endif()

include_directories(
  ${SFML_INCLUDE_DIR}
  ${CMAKE_SOURCE_DIR})

if(X11_FOUND)
  include_directories(${X11_INCLUDE_DIRS})
endif()


SET(DIAO_SRC
  EndScreen.cpp
  Enemy.cpp
  Main.cpp
  Maze.cpp
  MazeNode.cpp
  Player.cpp)

# Add OS specific subdirectories
if(APPLE)
  SET(DIAO_SRC ${DIAO_SRC} osx/ResourcePath.mm)
else()
  SET(${DIAO_SRC} other/ResourcePath.cpp)
endif()

if(WIN32)
  add_executable(DIAO WIN32 ${DIAO_SRC})
elseif(APPLE)
  add_executable(DIAO MACOSX_BUNDLE ${DIAO_SRC})
else()
  add_executable(DIAO ${DIAO_SRC})
endif()

target_link_libraries(DIAO
  ${SFML_LIBRARIES}
  ${SFML_DEPENDENCIES})

if(UNIX AND NOT APPLE)
  target_link_libraries(DIAO
    ${X11_LIBRARIES})
endif()

# Create a stand-alone bundle on OS X
if(APPLE)
  set(MAKE_INSTALL_NAME_DIR @rpath)

  # Add Apple libraries
  FIND_LIBRARY(FOUNDATION_LIBRARY Foundation)
  target_link_libraries(DIAO ${FOUNDATION_LIBRARY})

  #Add Info.plist to bundle
  set_target_properties(DIAO PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/osx/Info.plist.in)

  # Copy assets into the bundle
  set(ASSET_FILES
        ${PROJECT_SOURCE_DIR}/ALGER.TTF
        ${PROJECT_SOURCE_DIR}/floor.png
        ${PROJECT_SOURCE_DIR}/Minotaur.png
        ${PROJECT_SOURCE_DIR}/Player.png
        ${PROJECT_SOURCE_DIR}/SFML_LOGO.png
    )
  set(BUNDLE_PATH ${CMAKE_CURRENT_BINARY_DIR}/DIAO.app)
  install(FILES ${ASSET_FILES} DESTINATION ${BUNDLE_PATH}/Contents/Resources)
  install(FILES ${PROJECT_SOURCE_DIR}/osx/Icon.icns DESTINATION ${BUNDLE_PATH}/Contents/Resources)

  # Make the bundle drag-n-drop
  get_filename_component(SFML_LIBRARY_DIR ${SFML_SYSTEM_LIBRARY} DIRECTORY)
  set(LIBRARIES_DIR ${THOR_LIBRARY_DIR};${SFML_LIBRARY_DIR})
  # Copy freetype manually, since it otherwise just causes problems (I think because it uses @executable_path instead of @rpath)
  install(DIRECTORY ${SFML_LIBRARY_DIR}/freetype.framework DESTINATION ${BUNDLE_PATH}/Contents/Frameworks)
  # Let BundleUtilities do everything else automaitcally
  install(CODE "
    include(BundleUtilities)
    fixup_bundle(\"${BUNDLE_PATH}\" \"\" \"${LIBRARIES_DIR}\")
    " COMPONENT Runtime)
endif()
