# Install script for directory: D:/xudunke/navigation/recastnavigation-main/recastnavigation/Detour

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/RecastNavigation")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/xudunke/navigation/recastnavigation-main/recastnavigation/mybuild/Detour/Debug/Detour-d.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/xudunke/navigation/recastnavigation-main/recastnavigation/mybuild/Detour/Release/Detour.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/xudunke/navigation/recastnavigation-main/recastnavigation/mybuild/Detour/MinSizeRel/Detour.lib")
  elseif(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "D:/xudunke/navigation/recastnavigation-main/recastnavigation/mybuild/Detour/RelWithDebInfo/Detour.lib")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/recastnavigation" TYPE FILE FILES
    "D:/xudunke/navigation/recastnavigation-main/recastnavigation/Detour/Include/DetourAlloc.h"
    "D:/xudunke/navigation/recastnavigation-main/recastnavigation/Detour/Include/DetourAssert.h"
    "D:/xudunke/navigation/recastnavigation-main/recastnavigation/Detour/Include/DetourCommon.h"
    "D:/xudunke/navigation/recastnavigation-main/recastnavigation/Detour/Include/DetourMath.h"
    "D:/xudunke/navigation/recastnavigation-main/recastnavigation/Detour/Include/DetourNavMesh.h"
    "D:/xudunke/navigation/recastnavigation-main/recastnavigation/Detour/Include/DetourNavMeshBuilder.h"
    "D:/xudunke/navigation/recastnavigation-main/recastnavigation/Detour/Include/DetourNavMeshQuery.h"
    "D:/xudunke/navigation/recastnavigation-main/recastnavigation/Detour/Include/DetourNode.h"
    "D:/xudunke/navigation/recastnavigation-main/recastnavigation/Detour/Include/DetourStatus.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE FILE OPTIONAL FILES "D:/xudunke/navigation/recastnavigation-main/recastnavigation/mybuild/Detour/Debug/Detour-d.pdb")
  endif()
endif()
