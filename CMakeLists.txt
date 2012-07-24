cmake_minimum_required (VERSION 2.6)
Project (LPC2012Game)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

Find_Package (LibXml2 REQUIRED)
If (NOT LIBXML2_FOUND)
  Message (FATAL_ERROR "Couldn't find libxml2 development files. On Debian-based systems (such as Ubuntu) you should install the 'libxml2-dev' package.")
Else (NOT LIBXML2_FOUND)
  Include_Directories (${LIBXML2_INCLUDE_DIR})
EndIf (NOT LIBXML2_FOUND)

Find_Package(ALLEGRO5)
if(NOT ALLEGRO_FOUND)
  message(FATAL_ERROR "Couldn't find Allegro 5 and/or a required Allegro 5 module")
Else (NOT ALLEGRO_FOUNT)
  Include_Directories (${ALLEGRO_INCLUDE_DIR})
endif(NOT ALLEGRO_FOUND)

Set (FRAMEWORK_SOURCES 
	./src/framework/XmlHelper.cpp
	./src/framework/BitmapLoader.cpp
	./src/framework/Camera.cpp
	./src/framework/Director.cpp
	./src/framework/Drawable.cpp
	./src/framework/Scene.cpp
	./src/framework/Spritesheet.cpp
	./src/framework/TilemapLayer.cpp
	./src/framework/Button.cpp
	./src/framework/Label.cpp
)

Set (FRAMEWORK_INC 
	./src/framework/
)

Set (GAME_SOURCES 
	./src/model/Character.cpp
	./src/model/Memory.cpp
	./src/model/POI.cpp
	./src/model/Room.cpp
	./src/view/MainMenuScene.cpp
	./src/view/InvestigationScene.cpp
	./src/view/DebugCrimeScene.cpp
	./src/view/ModalDialog.cpp
	./src/controller/Mystery.cpp
)

Set (GAME_INC 
	./src/model/
	./src/view/
	./src/controller/
)

Set (LIB_INC
	./lib/astar/
)

Include_Directories(${LIB_INC} ${FRAMEWORK_INC} ${GAME_INC})

add_executable(LPC2012Game ${FRAMEWORK_SOURCES} ${GAME_SOURCES} ./src/main.cpp)

Target_Link_Libraries(LPC2012Game ${LIBXML2_LIBRARIES} ${ALLEGRO_LIBRARIES})