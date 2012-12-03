# CMakeLists.txt file for macros

# install all *.mac files in this directory
file(GLOB mac_files *.mac)
install( FILES ${mac_files}
         DESTINATION ${product}/${version}/${PROJECT_NAME}/macros )

# Also install in the build area
foreach(aFile ${mac_files})
  get_filename_component( basename ${aFile} NAME )
  configure_file( ${aFile} ${CMAKE_BINARY_DIR}/macros/${basename} COPYONLY )
endforeach(aFile)