#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "libevent::core" for configuration "Release"
set_property(TARGET libevent::core APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libevent::core PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/event_core.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/event_core.dll"
  )

list(APPEND _cmake_import_check_targets libevent::core )
list(APPEND _cmake_import_check_files_for_libevent::core "${_IMPORT_PREFIX}/lib/event_core.lib" "${_IMPORT_PREFIX}/lib/event_core.dll" )

# Import target "libevent::extra" for configuration "Release"
set_property(TARGET libevent::extra APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libevent::extra PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/event_extra.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/event_extra.dll"
  )

list(APPEND _cmake_import_check_targets libevent::extra )
list(APPEND _cmake_import_check_files_for_libevent::extra "${_IMPORT_PREFIX}/lib/event_extra.lib" "${_IMPORT_PREFIX}/lib/event_extra.dll" )

# Import target "libevent::openssl" for configuration "Release"
set_property(TARGET libevent::openssl APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(libevent::openssl PROPERTIES
  IMPORTED_IMPLIB_RELEASE "${_IMPORT_PREFIX}/lib/event_openssl.lib"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/event_openssl.dll"
  )

list(APPEND _cmake_import_check_targets libevent::openssl )
list(APPEND _cmake_import_check_files_for_libevent::openssl "${_IMPORT_PREFIX}/lib/event_openssl.lib" "${_IMPORT_PREFIX}/lib/event_openssl.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
