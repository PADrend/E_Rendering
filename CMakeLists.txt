#
# This file is part of the E_Rendering library.
# Copyright (C) 2009-2013 Benjamin Eikel <benjamin@eikel.org>
#
# This library is subject to the terms of the Mozilla Public License, v. 2.0.
# You should have received a copy of the MPL along with this library; see the 
# file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
#
cmake_minimum_required(VERSION 3.1.0)
project(E_Rendering VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set up install directories
include(GNUInstallDirs)
set(CMAKE_INSTALL_CMAKECONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/E_Rendering)

add_library(E_Rendering
	ELibRendering.cpp
	E_BufferObject.cpp
	E_Draw.cpp
	E_FBO.cpp
	E_OcclusionQuery.cpp
	E_ParameterStructs.cpp
	E_RenderingContext.cpp
	E_StatisticsQuery.cpp
	E_TextRenderer.cpp
	Mesh/E_Mesh.cpp
	Mesh/E_VertexAccessor.cpp
	Mesh/E_VertexAttribute.cpp
	Mesh/E_VertexAttributeAccessors.cpp
	Mesh/E_VertexDescription.cpp
	MeshUtils/E_MeshBuilder.cpp
	MeshUtils/E_MeshUtils.cpp
	MeshUtils/E_PlatonicSolids.cpp
	MeshUtils/E_PrimitiveShapes.cpp
	MeshUtils/E_TriangleAccessor.cpp
	MeshUtils/E_ConnectivityAccessor.cpp
	Shader/E_Shader.cpp
	Shader/E_Uniform.cpp
	Texture/E_Texture.cpp
	Texture/E_TextureUtils.cpp
)

# Dependency to EScript
if(NOT TARGET EScript)
	find_package(EScript 0.6.7 REQUIRED NO_MODULE)
endif()
target_link_libraries(E_Rendering LINK_PUBLIC EScript)

# Dependency to Geometry
if(NOT TARGET Geometry)
	find_package(Geometry 0.2.0 REQUIRED NO_MODULE)
endif()
target_link_libraries(E_Rendering LINK_PUBLIC Geometry)

# Dependency to Rendering
if(NOT TARGET Rendering)
	find_package(Rendering 0.2.0 REQUIRED NO_MODULE)
endif()
target_link_libraries(E_Rendering LINK_PUBLIC Rendering)

# Dependency to Util
if(NOT TARGET Util)
	find_package(Util 0.2.0 REQUIRED NO_MODULE)
endif()
target_link_libraries(E_Rendering LINK_PUBLIC Util)

# Dependency to E_Geometry
if(NOT TARGET E_Geometry)
	find_package(E_Geometry 0.2.0 REQUIRED NO_MODULE)
endif()
target_link_libraries(E_Rendering LINK_PUBLIC E_Geometry)

# Dependency to E_Util
if(NOT TARGET E_Util)
	find_package(E_Util 0.2.0 REQUIRED NO_MODULE)
endif()
target_link_libraries(E_Rendering LINK_PUBLIC E_Util)

# Set version of library
set_target_properties(E_Rendering PROPERTIES VERSION ${E_Rendering_VERSION}
                                             SOVERSION ${E_Rendering_VERSION_MAJOR})

# Install the header files
file(GLOB E_RENDERING_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/CL/*.h")
install(FILES ${E_RENDERING_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/E_Rendering/CL COMPONENT headers)
file(GLOB E_RENDERING_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Mesh/*.h")
install(FILES ${E_RENDERING_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/E_Rendering/Mesh COMPONENT headers)
file(GLOB E_RENDERING_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/MeshUtils/*.h")
install(FILES ${E_RENDERING_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/E_Rendering/MeshUtils COMPONENT headers)
file(GLOB E_RENDERING_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Shader/*.h")
install(FILES ${E_RENDERING_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/E_Rendering/Shader COMPONENT headers)
file(GLOB E_RENDERING_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/Texture/*.h")
install(FILES ${E_RENDERING_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/E_Rendering/Texture COMPONENT headers)
file(GLOB E_RENDERING_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
install(FILES ${E_RENDERING_HEADERS} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/E_Rendering COMPONENT headers)

# Install the library
install(TARGETS E_Rendering EXPORT LibraryExport
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT runtimelibraries
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT runtimelibraries
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT developmentlibraries
)

# Set include directories for users of this library
target_include_directories(E_Rendering
                           INTERFACE
                           "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>"
                           "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

# Library export
install(EXPORT LibraryExport DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR} FILE E_RenderingTargets.cmake COMPONENT developmentlibraries)

# Create the configuration files
include(CMakePackageConfigHelpers)
configure_package_config_file(E_RenderingConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/E_RenderingConfig.cmake
                              INSTALL_DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR}
                              PATH_VARS CMAKE_INSTALL_CMAKECONFIGDIR
                              NO_SET_AND_CHECK_MACRO
                              NO_CHECK_REQUIRED_COMPONENTS_MACRO)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/E_RenderingConfigVersion.cmake
                                 VERSION ${E_Rendering_VERSION}
                                 COMPATIBILITY SameMajorVersion)

# Install the configuration files
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/E_RenderingConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/E_RenderingConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_CMAKECONFIGDIR} COMPONENT developmentlibraries)

# Packaging
include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_NAME "librendering${E_Rendering_VERSION_MAJOR}-escript")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "E_Rendering library")
set(CPACK_PACKAGE_VENDOR "Benjamin Eikel; Claudius Jaehn; Ralf Petring")
set(CPACK_PACKAGE_CONTACT "Benjamin Eikel <benjamin@eikel.org>")
set(CPACK_PACKAGE_VERSION_MAJOR ${E_Rendering_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${E_Rendering_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${E_Rendering_VERSION_PATCH})
set(CPACK_PACKAGE_FILE_NAME "libE_Rendering")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_STRIP_FILES ON)

include(CPack)
