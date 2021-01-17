# CMakeList.txt : CMake project for CryptTools, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(project_name DetoursDrivers) ## rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 11)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/vendor/Detours/src
	${CMAKE_SOURCE_DIR}/DetoursDrivers
)
link_directories(
	${CMAKE_BINARY_DIR}/Detours
)

# Add source to this project's executable.
add_library (${project_name} SHARED
"dllmain.cpp"
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

target_link_libraries(${project_name} Detours)
# TODO: Add tests and install targets if needed.
