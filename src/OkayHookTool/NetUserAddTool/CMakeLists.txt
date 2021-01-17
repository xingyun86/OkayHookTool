# CMakeList.txt : CMake project for CryptTools, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(project_name NetUserAddTool) ## rename your project here

project(${project_name})

set(CMAKE_CXX_STANDARD 11)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR} 
	${CMAKE_SOURCE_DIR}/vendor/Common
)

# 把第三方库的.a文件和.so文件放到代码的lib文件夹
link_directories(
	${CMAKE_CURRENT_BINARY_DIR} 
)

# Add source to this project's executable.
add_executable(${project_name} "NetUserAddTool.cpp")

if (MSVC)
	list(APPEND project_name_cflags /W4)
	list(APPEND project_name_defines WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0501 _CRT_SECURE_NO_WARNINGS)
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

if(MSVC)
#SET_TARGET_PROPERTIES(${project_name} PROPERTIES LINK_FLAGS ${ADMIN_LEVEL})
endif(MSVC)
# TODO: Add tests and install targets if needed.
