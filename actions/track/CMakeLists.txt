# Track directory CMakeLists.txt

art_make( LIBRARIES "artg4_services_ActionHolder_service" 
      "artg4_pluginActions_physicalVolumeStore_physicalVolumeStore_service" 
      "gm2ringsim_common" "${XERCESCLIB}" "${allG4Libs}" )

add_subdirectory( arthits )

