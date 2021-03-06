#/*********************************************************\
# * Copyright (c) 2012-2013 Christian Ofenberg
# *
# * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
# * and associated documentation files (the "Software"), to deal in the Software without
# * restriction, including without limitation the rights to use, copy, modify, merge, publish,
# * distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
# * Software is furnished to do so, subject to the following conditions:
# *
# * The above copyright notice and this permission notice shall be included in all copies or
# * substantial portions of the Software.
# *
# * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
# * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
# * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
# * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#\*********************************************************/


cmake_minimum_required(VERSION 2.8.8)

find_package(Freetype REQUIRED)


##################################################
## Preprocessor definitions
##################################################
if(NOT RENDERER_DEBUG)
	add_definitions(-DRENDERER_NO_DEBUG)
endif()


##################################################
## Includes
##################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../Renderer/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${FREETYPE_INCLUDE_DIRS})


##################################################
## Source codes
##################################################
# Cross-platform source codes
set(SOURCE_CODES
	src/FontGlyphTextureFreeType.cpp
	src/FontTextureFreeType.cpp
	src/FreeTypeContext.cpp
	src/RendererToolkitImpl.cpp
)


##################################################
## Libraries
##################################################
if(STATIC_LIBRARY)
	add_library(RendererToolkitStatic STATIC ${SOURCE_CODES})
endif()
if(SHARED_LIBRARY)
	add_library(RendererToolkit SHARED ${SOURCE_CODES})
	set_target_properties(RendererToolkit PROPERTIES COMPILE_FLAGS "-D_USRDLL -DRENDERERTOOLKIT_EXPORTS")
	if(UNIX)
		target_link_libraries(RendererToolkit X11 dl ${FREETYPE_LIBRARY})
	endif()
endif()


##################################################
## Post build
##################################################
if(STATIC_LIBRARY)
	# Copy static library
	add_custom_command(TARGET RendererToolkitStatic
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_STATIC_LIBRARY_PREFIX}RendererToolkitStatic${CMAKE_STATIC_LIBRARY_SUFFIX}" ${OUTPUT_LIB_DIR}
	)
endif()
if(SHARED_LIBRARY)
	# Copy shared library
	add_custom_command(TARGET RendererToolkit
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/${CMAKE_SHARED_LIBRARY_PREFIX}RendererToolkit${CMAKE_SHARED_LIBRARY_SUFFIX}" ${OUTPUT_BIN_DIR}
	)
	if(WIN32)
		add_custom_command(TARGET RendererToolkit
			COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/RendererToolkit.lib" ${OUTPUT_LIB_DIR}
		)
	endif()
endif()

##################################################
## Post build
##################################################
# Copy header
if(SHARED_LIBRARY)
	add_custom_command(TARGET RendererToolkit
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/include/RendererToolkit/RendererToolkit.h" ${OUTPUT_INCLUDE_DIR}
	)
elseif(STATIC_LIBRARY)
	add_custom_command(TARGET RendererToolkitStatic
		COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/include/RendererToolkit/RendererToolkit.h" ${OUTPUT_INCLUDE_DIR}
	)
endif()
