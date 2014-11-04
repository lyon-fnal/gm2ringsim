# Track directory CMakeLists.txt

art_make( LIB_LIBRARIES
          artg4_services_ActionHolder_service
          artg4_pluginActions_physicalVolumeStore_physicalVolumeStore_service
          gm2ringsim_common 
          ${XERCESCLIB} 
	  ${G4_LIB_LIST}
        )

install_headers()
