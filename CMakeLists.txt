cmake_minimum_required(VERSION 3.3)
project(snoo CXX)

set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

find_package(Boost COMPONENTS system thread REQUIRED)
#todo
#find_package(OGRE REQUIRED)
#find_package(OIS REQUIRED)

include_directories(/usr/include/OGRE)

set(SOURCE_FILES
        src/ApplicationFramework.cpp    src/GameObjectFactory.cpp     src/MenuItem.cpp          src/RestoreBall.cpp
        src/BallBallCollision.cpp       src/GameStateChooseColor.cpp  src/MenuItemEdit.cpp      src/Runtime.cpp
        src/Ball.cpp                    src/GameStateCleanColor.cpp   src/MenuItemLabel.cpp     src/ShotEventGenerator.cpp
        src/BallCushionCollision.cpp    src/GameState.cpp             src/MenuItemPassword.cpp  src/ShotEvents.cpp
        src/BallPosition.cpp            src/GameStateEndOfGame.cpp    src/MenuItemSubmenu.cpp   src/SnooCommons.cpp
        src/BallPositions.cpp           src/GameStateFactory.cpp      src/NetworkClient.cpp     src/SnookerApplication.cpp
        src/Callbacks.cpp               src/GameStateFoul.cpp         src/NetworkPackage.cpp    src/SnookerClient.cpp
        src/Collision.cpp               src/GameStateInformation.cpp  src/NetworkServer.cpp     src/SnookerEventListener.cpp
        src/ComplexCushion.cpp          src/GameStatePotColor.cpp     src/NetworkSession.cpp    src/SnookerFrameListener.cpp
        src/Converter.cpp               src/GameStatePotRed.cpp       src/OISString.cpp         src/SnookerGui.cpp
        src/CushionDisk.cpp             src/GameStateTraining.cpp     src/Orientation.cpp       src/SnookerMessage.cpp
        src/CushionFace.cpp             src/Id.cpp                    src/OverlayCommons.cpp    src/SnookerModel.cpp
        src/Decision.cpp                src/main.cpp                  src/Physics.cpp           src/SnookerScene.cpp
        src/Encoder.cpp                 src/Menu.cpp                  src/Player.cpp            src/SnookerSettings.cpp
        src/FrameListenerFramework.cpp  src/MenuItemBool.cpp          src/Popup.cpp             src/Table.cpp
        src/GameObject.cpp              src/MenuItemCommand.cpp       src/R3.cpp                src/Utils.cpp)

execute_process(
        COMMAND
            pkg-config OGRE --variable=plugindir
        OUTPUT_VARIABLE
            OGRE_PLUGIN_FOLDER
)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data/plugins.cfg.in ${CMAKE_CURRENT_BINARY_DIR}/plugins.cfg @ONLY)

file(
        COPY
            ${CMAKE_CURRENT_SOURCE_DIR}/data/Media
        DESTINATION
            ${CMAKE_CURRENT_BINARY_DIR}
)
file(
        COPY
            ${CMAKE_CURRENT_SOURCE_DIR}/data/resources.cfg
        DESTINATION
            ${CMAKE_CURRENT_BINARY_DIR}
)

add_executable(snoo ${SOURCE_FILES})
target_link_libraries(snoo pthread OIS OgreMain OgreOverlay ${OGRE_LIBRARIES} ${OIS_LIBRARIES} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY})



