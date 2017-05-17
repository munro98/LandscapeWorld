
file(GLOB_RECURSE SRCS
	*.hpp
)

add_library(glm
	${SRCS}
)
set_target_properties(glm PROPERTIES LINKER_LANGUAGE CXX)
#target_include_directories(glm PUBLIC "${PROJECT_SOURCE_DIR}/ext")