macro(SETUP_GROUPS SRC_FILES)
	foreach(FILE ${SRC_FILES})
		get_filename_component(PARENT_DIR "${FILE}" PATH)

		# skip src or include and changes /'s to \\'s
		set(GROUP "${PARENT_DIR}")
		string(REPLACE "/" "\\" GROUP "${GROUP}")

		source_group("${GROUP}" FILES "${FILE}")
	endforeach()
endmacro()

function(buildExample EXAMPLE_NAME)
	set(EXAMPLE_FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/${EXAMPLE_NAME})
	message(STATUS "Generating project file for example in ${EXAMPLE_FOLDER}")

	# Add Main
	set(MAIN_CPP ${EXAMPLE_FOLDER}/${EXAMPLE_NAME}.cpp)
	if(EXISTS ${EXAMPLE_FOLDER}/main.cpp)
		set(MAIN_CPP ${EXAMPLE_FOLDER}/main.cpp)
	endif()
	if(EXISTS ${EXAMPLE_FOLDER}/${EXAMPLE_NAME}.h)
		set(MAIN_HEADER ${EXAMPLE_FOLDER}/${EXAMPLE_NAME}.h)
	endif()

	# Add sources and headers
	file(GLOB SRC_FILES  "*.cpp" ${EXAMPLE_FOLDER}/*.cpp)
	file(GLOB HEADER_FILES "*.h" ${EXAMPLE_FOLDER}/*.h)

	# Add shaders
	set(SHADER_DIR "../data/shaders/${EXAMPLE_NAME}")
	file(GLOB SHADER_FILES "${SHADER_DIR}/*.vert" "${SHADER_DIR}/*.frag" "${SHADER_DIR}/*.comp" "${SHADER_DIR_GLSL}/*.geom")
	
	SETUP_GROUPS("${SRC_FILES}")
	SETUP_GROUPS("${HEADER_FILES}")
	SETUP_GROUPS("${SHADER_FILES}")

	# Build executable
	if(WIN32)
		add_executable(${EXAMPLE_NAME} ${MAIN_CPP} ${MAIN_HEADER} ${SRC_FILES} ${HEADER_FILES} ${SHADER_FILES})
		target_link_libraries(${EXAMPLE_NAME} base )
	else(WIN32)
		add_executable(${EXAMPLE_NAME} ${MAIN_CPP} ${MAIN_HEADER} ${SRC_FILES} ${HEADER_FILES}  ${SHADER_FILES})
		target_link_libraries(${EXAMPLE_NAME} base )
	endif(WIN32)

	# Link third party
	target_link_libraries(${EXAMPLE_NAME} glad glfw imgui)

	# Set properties
	set_target_properties(${EXAMPLE_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

	if(RESOURCE_INSTALL_DIR)
		install(TARGETS ${EXAMPLE_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
	endif()

	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../base)

endfunction(buildExample)

# Build all examples
function(buildExamples)
	foreach(EXAMPLE ${EXAMPLES})
		buildExample(${EXAMPLE})
	endforeach(EXAMPLE)
endfunction(buildExamples)

set(EXAMPLES
	triangle
	grayfilter
)

buildExamples()




