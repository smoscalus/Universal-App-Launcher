#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "hellnah::hellnah_db" for configuration ""
set_property(TARGET hellnah::hellnah_db APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(hellnah::hellnah_db PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libhellnah_db.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS hellnah::hellnah_db )
list(APPEND _IMPORT_CHECK_FILES_FOR_hellnah::hellnah_db "${_IMPORT_PREFIX}/lib/libhellnah_db.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
