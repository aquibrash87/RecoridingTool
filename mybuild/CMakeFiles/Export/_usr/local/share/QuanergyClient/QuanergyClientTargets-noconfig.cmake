#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "quanergy_client" for configuration ""
set_property(TARGET quanergy_client APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(quanergy_client PROPERTIES
  IMPORTED_LOCATION_NOCONFIG "/usr/local/lib/libquanergy_client.so"
  IMPORTED_SONAME_NOCONFIG "libquanergy_client.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS quanergy_client )
list(APPEND _IMPORT_CHECK_FILES_FOR_quanergy_client "/usr/local/lib/libquanergy_client.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
