cmake_minimum_required (VERSION 2.6)
project (image)

set(SOURCES
		consumer/image_consumer.cpp

		producer/image_producer.cpp
		producer/image_scroll_producer.cpp

		util/image_algorithms.cpp
		util/image_loader.cpp

		image.cpp
)
set(HEADERS
		consumer/image_consumer.h

		producer/image_producer.h
		producer/image_scroll_producer.h

		util/image_algorithms.h
		util/image_loader.h
		util/image_view.h

		image.h
)

add_library(image ${SOURCES} ${HEADERS})

include_directories(..)
include_directories(../..)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${FREEIMAGE_INCLUDE_DIRS})
include_directories(${TBB_INCLUDE_DIRS})

set_target_properties(image PROPERTIES FOLDER modules)
source_group(sources\\consumer consumer/*)
source_group(sources\\producer producer/*)
source_group(sources\\util util/*)
source_group(sources ./*)

target_link_libraries(image
	common
	core

	"${FREEIMAGE_LIBRARIES}"
)

casparcg_add_include_statement("modules/image/image.h")
casparcg_add_init_statement("image::init" "image")
casparcg_add_uninit_statement("image::uninit")
casparcg_add_module_project("image")

if(NOT USE_SYSTEM_FREEIMAGE)
	if(MSVC)
		casparcg_add_runtime_dependency("${FREEIMAGE_BIN_DIR}/FreeImage.dll")
		casparcg_add_runtime_dependency("${FREEIMAGE_BIN_DIR}/FreeImaged.dll")
	else()
		casparcg_add_runtime_dependency("${FREEIMAGE_BIN_DIR}/libfreeimage.so.3")
	endif()
endif()
