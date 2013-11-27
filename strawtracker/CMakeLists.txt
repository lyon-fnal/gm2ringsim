# Build the libraries
art_make( SERVICE_LIBRARIES "artg4_util" "gm2ringsim_strawtracker" "artg4_material" "gm2geom_strawtracker" "gm2geom_vac" "gm2geom_Core_Geometry_service" "gm2dataproducts_strawtracker")

# Copy the headers
install_headers()

# Copy the fcl
# install all *.fcl files in the fcl/geom directory
file(GLOB fcl_files *.fcl)
install( FILES ${fcl_files}
         DESTINATION ${product}/${version}/${PROJECT_NAME}/fcl/geom )

# Also install in the build area
foreach(aFile ${fcl_files})
  get_filename_component( basename ${aFile} NAME )
  configure_file( ${aFile} ${CMAKE_BINARY_DIR}/fcl/geom/${basename} COPYONLY )
endforeach(aFile)
