#art_make( SERVICE_LIBRARIES LIB_LIBRARIES "artg4_util" "gm2ringsim_inflector" "gm2ringsim_common" "gm2ringsim_common_spin" "gm2ringsim_fields" "gm2ringsim_actions" "gm2geom_inflector")

#art_make( SERVICE_LIBRARIES "gm2ringsim_inflector" LIB_LIBRARIES "gm2ringsim_fields" "artg4_util" "gm2ringsim_common" "gm2ringsim_common_spin" "gm2ringsim_fields" "gm2ringsim_actions" "gm2geom_inflector")

art_make( LIB_LIBRARIES 
          artg4_pluginActions_physicalVolumeStore_physicalVolumeStore_service
	  gm2ringsim_fields 
	  gm2ringsim_inflector
	  artg4_util   
	  gm2ringsim_common
	  gm2ringsim_common_spin   
	  gm2ringsim_fields
	  gm2ringsim_actions   
	  gm2geom_inflector 
	)

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
