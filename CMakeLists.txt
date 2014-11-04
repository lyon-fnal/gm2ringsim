# ======================================================================
#  gm2ringsim main build file
#
#  . mrb s
#  mrb b
#
# ======================================================================


# use cmake 2.8 or later
cmake_minimum_required (VERSION 2.8)

project(gm2ringsim)

# cetbuildtools contains our cmake modules
SET ( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
IF (NOT CETBUILDTOOLS_VERSION)
MESSAGE (FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules")
ENDIF()

set(CMAKE_MODULE_PATH $ENV{ART_DIR}/Modules
$ENV{CETBUILDTOOLS_DIR}/Modules
${CMAKE_MODULE_PATH})

include(CetCMakeEnv)
cet_cmake_env()

set_install_root()

# g-2 wants the product name in the library name
set (ART_MAKE_PREPEND_PRODUCT_NAME TRUE)

if ( ${APPLE} )
cet_set_compiler_flags(DIAGS CAUTIOUS
WERROR
EXTRA_FLAGS -pedantic

)
else()
cet_set_compiler_flags(DIAGS CAUTIOUS
WERROR
EXTRA_FLAGS -pedantic

)
endif()

cet_report_compiler_flags()


# Locate some UPS areas (the versions are just minimum required) and setup include paths
# These are needed for Art stuff
find_ups_product(cetbuildtools v4_03_01)
find_ups_product(gm2geom v2_00_00)
find_ups_product(gm2dataproducts v2_00_00)
find_ups_product(artg4 v2_00_00)
find_ups_geant4(v4_9_6_p03)
find_ups_product(art v1_12_02 )
find_ups_product(fhiclcpp v3_01_02)
find_ups_product(messagefacility v1_11_15)

# macros for dictionary and simple_plugin
include(ArtDictionary)
include(ArtMake)
include(BuildPlugins)

# Build these directories

add_subdirectory( fcl )
add_subdirectory( macros )
add_subdirectory( world )
add_subdirectory( arc )
add_subdirectory( vac )
add_subdirectory( fields )
add_subdirectory( inflector )
add_subdirectory( quad )
add_subdirectory( octupole )
add_subdirectory( kicker )
add_subdirectory( common )
add_subdirectory( ups )
add_subdirectory( test )
add_subdirectory( actions )
add_subdirectory( strawtracker )
add_subdirectory( fiberHarp )
add_subdirectory( station )
add_subdirectory( calo )
add_subdirectory( collimator )
add_subdirectory (exampleAnalyses)
add_subdirectory( g2RunTimeFiles )

# Packaging facility
include(UseCPack)
