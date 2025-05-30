# Generated by CMake

if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.8)
   message(FATAL_ERROR "CMake >= 2.8.0 required")
endif()
if(CMAKE_VERSION VERSION_LESS "2.8.3")
   message(FATAL_ERROR "CMake >= 2.8.3 required")
endif()
cmake_policy(PUSH)
cmake_policy(VERSION 2.8.3...3.24)
#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Protect against multiple inclusion, which would fail when already imported targets are added once more.
set(_cmake_targets_defined "")
set(_cmake_targets_not_defined "")
set(_cmake_expected_targets "")
foreach(_cmake_expected_target IN ITEMS libevent::core libevent::extra libevent::openssl)
  list(APPEND _cmake_expected_targets "${_cmake_expected_target}")
  if(TARGET "${_cmake_expected_target}")
    list(APPEND _cmake_targets_defined "${_cmake_expected_target}")
  else()
    list(APPEND _cmake_targets_not_defined "${_cmake_expected_target}")
  endif()
endforeach()
unset(_cmake_expected_target)
if(_cmake_targets_defined STREQUAL _cmake_expected_targets)
  unset(_cmake_targets_defined)
  unset(_cmake_targets_not_defined)
  unset(_cmake_expected_targets)
  unset(CMAKE_IMPORT_FILE_VERSION)
  cmake_policy(POP)
  return()
endif()
if(NOT _cmake_targets_defined STREQUAL "")
  string(REPLACE ";" ", " _cmake_targets_defined_text "${_cmake_targets_defined}")
  string(REPLACE ";" ", " _cmake_targets_not_defined_text "${_cmake_targets_not_defined}")
  message(FATAL_ERROR "Some (but not all) targets in this export set were already defined.\nTargets Defined: ${_cmake_targets_defined_text}\nTargets not yet defined: ${_cmake_targets_not_defined_text}\n")
endif()
unset(_cmake_targets_defined)
unset(_cmake_targets_not_defined)
unset(_cmake_expected_targets)


# Create imported target libevent::core
add_library(libevent::core SHARED IMPORTED)

set_target_properties(libevent::core PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "D:/work_mine/3rd_libs/libevent-2.1.12-stable/include;D:/work_mine/3rd_libs/libevent-2.1.12-stable/mybuild/include"
  INTERFACE_LINK_LIBRARIES "ws2_32;shell32;advapi32;iphlpapi"
)

# Create imported target libevent::extra
add_library(libevent::extra SHARED IMPORTED)

set_target_properties(libevent::extra PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "D:/work_mine/3rd_libs/libevent-2.1.12-stable/include;D:/work_mine/3rd_libs/libevent-2.1.12-stable/mybuild/include"
  INTERFACE_LINK_LIBRARIES "ws2_32;shell32;advapi32;libevent::core"
)

# Create imported target libevent::openssl
add_library(libevent::openssl SHARED IMPORTED)

set_target_properties(libevent::openssl PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES "D:/work_mine/3rd_libs/libevent-2.1.12-stable/include;D:/work_mine/3rd_libs/libevent-2.1.12-stable/mybuild/include;D:/work_mine/3rd_libs/openssl-1.1.1s/include"
  INTERFACE_LINK_LIBRARIES "ws2_32;shell32;advapi32;libevent::core;D:/work_mine/3rd_libs/openssl-1.1.1s/libssl.lib;D:/work_mine/3rd_libs/openssl-1.1.1s/libcrypto.lib"
)

# Import target "libevent::core" for configuration "Debug"
set_property(TARGET libevent::core APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(libevent::core PROPERTIES
  IMPORTED_IMPLIB_DEBUG "D:/work_mine/3rd_libs/libevent-2.1.12-stable/mybuild/lib/Debug/event_core.lib"
  IMPORTED_LOCATION_DEBUG "D:/work_mine/3rd_libs/libevent-2.1.12-stable/mybuild/bin/Debug/event_core.dll"
  )

# Import target "libevent::extra" for configuration "Debug"
set_property(TARGET libevent::extra APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(libevent::extra PROPERTIES
  IMPORTED_IMPLIB_DEBUG "D:/work_mine/3rd_libs/libevent-2.1.12-stable/mybuild/lib/Debug/event_extra.lib"
  IMPORTED_LOCATION_DEBUG "D:/work_mine/3rd_libs/libevent-2.1.12-stable/mybuild/bin/Debug/event_extra.dll"
  )

# Import target "libevent::openssl" for configuration "Debug"
set_property(TARGET libevent::openssl APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(libevent::openssl PROPERTIES
  IMPORTED_IMPLIB_DEBUG "D:/work_mine/3rd_libs/libevent-2.1.12-stable/mybuild/lib/Debug/event_openssl.lib"
  IMPORTED_LOCATION_DEBUG "D:/work_mine/3rd_libs/libevent-2.1.12-stable/mybuild/bin/Debug/event_openssl.dll"
  )

# Import target "libevent::core" for configuration "Release"
set_property(TARGET libevent::core APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libevent::core PROPERTIES
  IMPORTED_IMPLIB_RELEASE "D:/work_mine/3rd_libs/libevent-2.1.12-stable/mybuild/lib/Release/event_core.lib"
  IMPORTED_LOCATION_RELEASE "D:/work_mine/3rd_libs/libevent-2.1.12-stable/mybuild/bin/Release/event_core.dll"
  )

# Import target "libevent::extra" for configuration "Release"
set_property(TARGET libevent::extra APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libevent::extra PROPERTIES
  IMPORTED_IMPLIB_RELEASE "D:/work_mine/3rd_libs/libevent-2.1.12-stable/mybuild/lib/Release/event_extra.lib"
  IMPORTED_LOCATION_RELEASE "D:/work_mine/3rd_libs/libevent-2.1.12-stable/mybuild/bin/Release/event_extra.dll"
  )

# Import target "libevent::openssl" for configuration "Release"
set_property(TARGET libevent::openssl APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libevent::openssl PROPERTIES
  IMPORTED_IMPLIB_RELEASE "D:/work_mine/3rd_libs/libevent-2.1.12-stable/mybuild/lib/Release/event_openssl.lib"
  IMPORTED_LOCATION_RELEASE "D:/work_mine/3rd_libs/libevent-2.1.12-stable/mybuild/bin/Release/event_openssl.dll"
  )

# This file does not depend on other imported targets which have
# been exported from the same project but in a separate export set.

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
cmake_policy(POP)
