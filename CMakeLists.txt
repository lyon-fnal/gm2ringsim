#  This file is the main CMakeLists.txt file for the @gm2ringsim@ package (an art module package)

# h2. Introduction 

#  Before running cmake, you must ensure that all external
#  packages are set up already (e.g. source all necessary setup
#  files, including the appropriate @setup_for_development@ script).

#  If you are copying this project to use as a template for a new project, you will need to make some minor changes. The 
#  areas where you would have to do so are marked with -->

#  This most important place where you would have to add to this file
#  is at the bottom.

# h2. Main code

# Ensure we are using a moden version of CMake
CMAKE_MINIMUM_REQUIRED (VERSION 2.8)

# h3. Project definition

# --> Define the name of this project (replace @gm2artexamples@ with your 
# project name - use all lowercase)
PROJECT (gm2ringsim)

# h3. Load build system modules
set( CETBUILDTOOLS_VERSION $ENV{CETBUILDTOOLS_VERSION} )
if( NOT CETBUILDTOOLS_VERSION )
  message( FATAL_ERROR "ERROR: setup cetbuildtools to get the cmake modules" )
endif()
set( CMAKE_MODULE_PATH $ENV{CETBUILDTOOLS_DIR}/Modules ${CMAKE_MODULE_PATH} )

# art contains cmake modules that we use
set( ART_VERSION $ENV{ART_VERSION} )
if( NOT ART_VERSION )
  message( FATAL_ERROR "ERROR: setup art to get the cmake modules" )
endif()
set( CMAKE_MODULE_PATH $ENV{ART_DIR}/Modules ${CMAKE_MODULE_PATH} )

# Include the necessary macros
include(CetCMakeEnv)
include(BuildPlugins)
include(ArtMake)
include(FindUpsGeant4)

# h3. Configure this product

# Configure
cet_cmake_env()

# Set compiler flags
cet_set_compiler_flags( DIAGS VIGILANT WERROR
   EXTRA_FLAGS -pedantic
   EXTRA_CXX_FLAGS -std=c++11
)

cet_report_compiler_flags()


# Locate some UPS areas (the versions are just minimum required) and setup include paths
# These are needed for Art stuff
find_ups_product(cetbuildtools v3_13_00)
find_ups_product(gm2geom v2_00_00)
find_ups_product(gm2dataproducts v2_00_00)
find_ups_product(artg4 v2_00_00)
find_ups_geant4(v4_9_6_p03)
find_ups_product(art v1_10_00 )
find_ups_product(fhiclcpp v2_19_05)
find_ups_product(messagefacility v1_11_10)

# Make sure we have gcc
cet_check_gcc()

# Macros for art_make and simple plugins
include(ArtDictionary)

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
