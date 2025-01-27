cmake_minimum_required (VERSION 2.6)
project (html)

set(SOURCES
		producer/html_cg_proxy.cpp
		producer/html_producer.cpp

		html.cpp
)
set(HEADERS
		producer/html_cg_proxy.h
		producer/html_producer.h

		html.h
)

add_library(html ${SOURCES} ${HEADERS})

include_directories(..)
include_directories(../..)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${TBB_INCLUDE_DIRS})
include_directories(${CEF_INCLUDE_DIRS})
include_directories(${CEF_PATH})

set_target_properties(html PROPERTIES FOLDER modules)
source_group(sources\\producer producer/*)
source_group(sources ./*)

target_link_libraries(html
	common
	core
	"${CEF_LIBRARIES}"
)

# Build and link in libcef_dll_wrapper.
set(CEF_ROOT "${CEF_PATH}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CEF_PATH}/cmake")
find_package(CEF REQUIRED)
add_subdirectory(${CEF_LIBCEF_DLL_WRAPPER_PATH} libcef_dll_wrapper)


# libcef_dll_wrapper exports a set of compiler flags that are not
# compatible with e.g. Boost.Log, so we need to reset them here.
SET_TARGET_PROPERTIES(libcef_dll_wrapper PROPERTIES INTERFACE_COMPILE_OPTIONS "")

casparcg_add_include_statement("modules/html/html.h")
casparcg_add_init_statement("html::init" "html")
casparcg_add_uninit_statement("html::uninit")
casparcg_add_command_line_arg_interceptor("html::intercept_command_line")
casparcg_add_module_project("html")

casparcg_add_runtime_dependency("${CEF_RESOURCE_PATH}/locales")
casparcg_add_runtime_dependency("${CEF_RESOURCE_PATH}/cef.pak")
casparcg_add_runtime_dependency("${CEF_RESOURCE_PATH}/cef_100_percent.pak")
casparcg_add_runtime_dependency("${CEF_RESOURCE_PATH}/cef_200_percent.pak")
casparcg_add_runtime_dependency("${CEF_RESOURCE_PATH}/cef_extensions.pak")
casparcg_add_runtime_dependency("${CEF_RESOURCE_PATH}/devtools_resources.pak")
casparcg_add_runtime_dependency("${CEF_RESOURCE_PATH}/icudtl.dat")
casparcg_add_runtime_dependency("${CEF_BIN_PATH}/natives_blob.bin")
casparcg_add_runtime_dependency("${CEF_BIN_PATH}/snapshot_blob.bin")
casparcg_add_runtime_dependency("${CEF_BIN_PATH}/swiftshader")

if(MSVC)
	casparcg_add_runtime_dependency("${CEF_BIN_PATH}/libcef.dll")
	casparcg_add_runtime_dependency("${CEF_BIN_PATH}/chrome_elf.dll")
	casparcg_add_runtime_dependency("${CEF_BIN_PATH}/d3dcompiler_43.dll")
	casparcg_add_runtime_dependency("${CEF_BIN_PATH}/d3dcompiler_47.dll")
	casparcg_add_runtime_dependency("${CEF_BIN_PATH}/libEGL.dll")
	casparcg_add_runtime_dependency("${CEF_BIN_PATH}/libGLESv2.dll")
else()
	casparcg_add_runtime_dependency("${CEF_BIN_PATH}/chrome-sandbox")
	casparcg_add_runtime_dependency("${CEF_BIN_PATH}/libcef.so")
	casparcg_add_runtime_dependency("${CEF_BIN_PATH}/libwidevinecdmadapter.so")
endif()

