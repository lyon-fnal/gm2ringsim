# Build the libraries
art_make( LIB_LIBRARIES 
	  gm2ringsim_calo
	  gm2geom_calo
	  gm2geom_station
	  artg4_material
	  artg4_util 
	  ${XERCESCLIB}
	  ${G4_LIB_LIST}
	)

# Copy the headers
install_headers()

# Copy the fcl
# install all *.fcl files in the fcl/geom directory
file(GLOB fcl_files *.fcl)
install( FILES ${fcl_files}
         DESTINATION ${product}/${version}/fcl/geom )

# Also install in the build area
foreach(aFile ${fcl_files})
  get_filename_component( basename ${aFile} NAME )
  configure_file( ${aFile} ${CMAKE_BINARY_DIR}/${product}/fcl/geom/${basename} COPYONLY )
endforeach(aFile)
