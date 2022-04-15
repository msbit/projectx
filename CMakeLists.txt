cmake_minimum_required(VERSION 3.10)

project(ProjectX VERSION 1.0)

set(CMAKE_CXX_EXTENSIONS OFF)
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -fno-omit-frame-pointer")
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(ProjectX
  headers/HouseSceneReducer.h
  headers/Item.h
  headers/TileLayer.h
  headers/Animation.h
  headers/HouseScene.h
  headers/Map.h
  headers/SpriteSheet.h
  headers/Entity.h
  headers/TilePaletteView.h
  headers/TilePaletteTile.h
  headers/Controller.h
  headers/EditorController.h
  headers/PlayerController.h
  headers/View.h
  headers/EventWithMouse.h
  headers/HouseSceneState.h
  headers/EntityView.h
  headers/GridView.h
  headers/TileBackgroundView.h
  headers/ViewLayer.h
  headers/SelectedTileView.h
  headers/BoxSelectionView.h
  headers/GridSelectionView.h
  headers/ToolbarToolsView.h
  headers/ToolbarController.h
  headers/CreatedRoomSelectionView.h
  headers/ShadowView.h
  headers/AssetWatcher.h
  headers/EntityFactory.h
  headers/ControllerScheme.h
  headers/KeyboardControllerScheme.h
  headers/GamepadControllerScheme.h
  headers/Screen.h
  headers/ScreenValues.h
  headers/HouseSceneFactory.h
  headers/AIController.h
  headers/Physics.h
  src/Animation.cpp
  src/HouseScene.cpp
  src/Map.cpp
  src/SpriteSheet.cpp
  src/main.cpp
  src/TilePaletteView.cpp
  src/EditorController.cpp
  src/PlayerController.cpp
  src/HouseSceneReducer.cpp
  src/EntityView.cpp
  src/GridView.cpp
  src/TileBackgroundView.cpp
  src/ViewLayer.cpp
  src/SelectedTileView.cpp
  src/BoxSelectionView.cpp
  src/GridSelectionView.cpp
  src/ToolbarToolsView.cpp
  src/ToolbarController.cpp
  src/CreatedRoomSelectionView.cpp
  src/ShadowView.cpp
  src/AssetWatcher.common.cpp
  src/EntityFactory.cpp
  src/KeyboardControllerScheme.cpp
  src/GamepadControllerScheme.cpp
  src/Screen.cpp
  src/HouseSceneFactory.cpp
  src/AIController.cpp
  src/Physics.cpp
)

if(WIN32)
  add_definitions(-DWIN32)
  target_sources(ProjectX
    PRIVATE
    src/AssetWatcher.win32.cpp
  )
elseif(APPLE)
  add_definitions(-DAPPLE)
  target_sources(ProjectX
    PRIVATE
    src/AssetWatcher.apple.cpp
  )
elseif(CMAKE_SYSTEM_INFO_FILE STREQUAL "Platform/Linux")
  add_definitions(-DLINUX)
  target_sources(ProjectX
    PRIVATE
    src/AssetWatcher.linux.cpp
  )
else()
  target_sources(ProjectX
    PRIVATE
    src/AssetWatcher.stub.cpp
  )
endif()

target_include_directories(ProjectX
  PRIVATE headers
)
target_link_directories(ProjectX
  PRIVATE /usr/lib/debug/usr/lib
)
if(NOT DEFINED SFML_LIBRARY_SUFFIX)
  set(SFML_LIBRARY_SUFFIX -d)
endif()
target_link_libraries(ProjectX
  sfml-graphics${SFML_LIBRARY_SUFFIX}
  sfml-system${SFML_LIBRARY_SUFFIX}
  sfml-window${SFML_LIBRARY_SUFFIX}
)

if(DEFINED SFML_PREFIX)
  target_include_directories(ProjectX
    PRIVATE ${SFML_PREFIX}/include
  )
  target_link_directories(ProjectX
    PRIVATE ${SFML_PREFIX}/lib
  )
endif()

if(DEFINED ENV{SFML_PREFIX})
  target_include_directories(ProjectX
    PRIVATE $ENV{SFML_PREFIX}/include
  )
  target_link_directories(ProjectX
    PRIVATE $ENV{SFML_PREFIX}/lib
  )
endif()

find_package(Threads REQUIRED)
target_link_libraries(ProjectX
  Threads::Threads
)
