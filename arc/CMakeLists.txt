# Build the libraries
art_make( SERVICE_LIBRARIES
	  gm2ringsim_arc
	   LIB_LIBRARIES 
	  gm2ringsim_arc
	  gm2geom_arc
	  gm2ringsim_inflector
	  gm2ringsim_actions
	  gm2ringsim_fields
	  artg4_util
	  artg4_material
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
