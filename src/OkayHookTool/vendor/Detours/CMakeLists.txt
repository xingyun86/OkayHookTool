# CMakeList.txt : CMake project for CryptTools, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(project_name Detours) ## rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 11)

include_directories(
${CMAKE_SOURCE_DIR}/Detours
${CMAKE_SOURCE_DIR}/Detours/src
)

IF(CMAKE_CL_64)
    set(platform x64)
	set(EXT_SRC "src/disolx64.cpp")
ELSE(CMAKE_CL_64)
    set(platform x86)
	set(EXT_SRC "src/disolx86.cpp")
ENDIF(CMAKE_CL_64)

# Add source to this project's executable.
add_library (${project_name} STATIC
"src/creatwth.cpp" 
"src/detours.cpp" 
"src/disasm.cpp" 
"src/image.cpp" 
"src/modules.cpp" 
${EXT_SRC}
)

if (MSVC)
	list(APPEND project_name_cflags /W4)
	list(APPEND project_name_defines WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0501)
	MESSAGE(STATUS "CMAKE_BUILD_TYPE=" ${CMAKE_BUILD_TYPE})

	set(CompilerFlags
		CMAKE_CXX_FLAGS
		CMAKE_CXX_FLAGS_DEBUG
		CMAKE_CXX_FLAGS_RELEASE
		CMAKE_C_FLAGS
		CMAKE_C_FLAGS_DEBUG
		CMAKE_C_FLAGS_RELEASE
		)
	foreach(CompilerFlag ${CompilerFlags})
		string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
	endforeach()
	
	target_compile_definitions(${project_name}
							PRIVATE ${project_name_defines}
							)
	target_compile_options(${project_name} 
							PRIVATE ${project_name_cflags}
	)
endif(MSVC)


# TODO: Add tests and install targets if needed.
