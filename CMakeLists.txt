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

# cetbuildtools contains our cmake modules


include(CetCMakeEnv)
include(BuildPlugins)
include(FindUpsGeant4)

# h3. Configure this product

# Configure
cet_cmake_env()

# Set compiler flags
cet_set_compiler_flags( DIAGS VIGILANT WERROR
#   NO_UNDEFINED
   EXTRA_FLAGS -pedantic
   EXTRA_CXX_FLAGS -std=c++11
)
cet_have_qual( sse2 SSE2 )
if ( SSE2 )
  cet_add_compiler_flags(CXX -msse2 -ftree-vectorizer-verbose=2)
endif()

cet_report_compiler_flags()


# Locate some UPS areas (the versions are just minimum required) and setup include paths
# These are needed for Art stuff
#find_ups_product(artg4 v0_0_1)   ADD THIS WHEN ARTG4 IS RELEASED
find_ups_product(cetbuildtools v3_06_02)
find_ups_product(xerces_c v3_1_1)
find_ups_geant4( v4_9_6_p02)
find_ups_product(cpp0x v1_03_23)
find_ups_product(libsigcpp v2_2_11)
find_ups_product(cppunit v1_12_1)
find_ups_product(clhep v2_1_3_1)
find_ups_product(cetlib v1_03_23)
find_ups_product(gccxml v0_9_20130621)
find_ups_product(art v1_08_09)
find_ups_product(fhiclcpp v2_17_10)
find_ups_product(messagefacility v1_10_24)
find_ups_root( v5_34_09 )
find_ups_boost( v1_53_0)

#add_definitions(-DG4_STORE_TRAJECTORY -DG4VERBOSE -DG4UI_USE -DG4VIS_USE -DG4UI_USE_TCSH -DG4INTY_USE_XT -DG4VIS_USE_OPENGLX -DG4VIS_USE_OPENGL)

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
#add_subdirectory (exampleAnalyses)

# Packaging facility
include(UseCPack)


