cmake_minimum_required (VERSION 2.6)
project (bluefish)

set(SOURCES
		consumer/bluefish_consumer.cpp
		producer/bluefish_producer.cpp
		util/blue_velvet.cpp
		bluefish.cpp
		StdAfx.cpp
)
set(HEADERS
		consumer/bluefish_consumer.h
		producer/bluefish_producer.h
		util/blue_velvet.h
		util/memory.h
		bluefish.h
		StdAfx.h
		interop/BlueVelvetCUtils.h
)

add_library(bluefish ${SOURCES} ${HEADERS})
add_precompiled_header(bluefish StdAfx.h FORCEINCLUDE)

include_directories(..)
include_directories(../..)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${TBB_INCLUDE_DIRS})
include_directories(${FFMPEG_INCLUDE_DIRS})
include_directories(${BLUEFISH_INCLUDE_DIRS})

set_target_properties(bluefish PROPERTIES FOLDER modules)
source_group(sources ./*)
source_group(sources\\consumer consumer/*)
source_group(sources\\producer producer/*)
source_group(sources\\util util/*)

target_link_libraries(	bluefish
			common 
			core
			ffmpeg)

casparcg_add_include_statement("modules/bluefish/bluefish.h")
casparcg_add_init_statement("bluefish::init" "bluefish")
casparcg_add_module_project("bluefish")
