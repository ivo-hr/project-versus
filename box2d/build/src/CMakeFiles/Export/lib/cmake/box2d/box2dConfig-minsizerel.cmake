#----------------------------------------------------------------
# Generated CMake target import file for configuration "MinSizeRel".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "box2d::box2d" for configuration "MinSizeRel"
set_property(TARGET box2d::box2d APPEND PROPERTY IMPORTED_CONFIGURATIONS MINSIZEREL)
set_target_properties(box2d::box2d PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_MINSIZEREL "CXX"
  IMPORTED_LOCATION_MINSIZEREL "${_IMPORT_PREFIX}/lib/box2d.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS box2d::box2d )
list(APPEND _IMPORT_CHECK_FILES_FOR_box2d::box2d "${_IMPORT_PREFIX}/lib/box2d.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
