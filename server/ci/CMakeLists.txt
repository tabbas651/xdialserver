##########################################################################
# If not stated otherwise in this file or this component's Licenses.txt
# file the following copyright and licenses apply:
#
# Copyright 2019 RDK Management
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
##########################################################################

find_package(PkgConfig REQUIRED)
#
# Implementation specific dependencies
#
pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_search_module(GOBJECT REQUIRED gobject-2.0)
set (GDIAL_PLAT_DEPEND_LIBRARIES "${GLIB_LIBRARIES} ${GOBJECT_LIBRARIES}")

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS} -std=c++11 -Wno-nonnull -DRT_PLATFORM_LINUX")

#
# Template that generates .pc file
#
set (GDIAL_PLAT_PC_LIBNAME "gdial-plat")
set (GDIAL_PLAT_PC_FILENAME ${PROJECT_SOURCE_DIR}/plat/gdial-plat.pc)
string(REGEX REPLACE " " ";" GDIAL_PLAT_DEPEND_LIBRARIES "${GDIAL_PLAT_DEPEND_LIBRARIES}")
foreach (deplib ${GDIAL_PLAT_DEPEND_LIBRARIES})
  set (GDIAL_PLAT_PC_LIBS_PRIVATE "${GDIAL_PLAT_PC_LIBS_PRIVATE} -l${deplib} ")
endforeach(deplib)
configure_file ("${GDIAL_PLAT_PC_FILENAME}.in" "${GDIAL_PLAT_PC_FILENAME}" @ONLY)

#
# Build target library
#
include_directories(
  ${GLIB_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/linux
  ${CMAKE_CURRENT_SOURCE_DIR}
)

set (GDIAL_PLAT_LIB_SOURCE_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/../linux/gdial-plat-dev.c
  ${CMAKE_CURRENT_SOURCE_DIR}/../linux/gdial-plat-util.c
  ${CMAKE_CURRENT_SOURCE_DIR}/gdial-plat-app.c
)

add_library(gdial-plat SHARED ${GDIAL_PLAT_LIB_SOURCE_FILES})
target_link_Libraries(gdial-plat PRIVATE ${GLIB_LIBRARIES} ${GOBJECT_LIBRARIES} -lpthread)
