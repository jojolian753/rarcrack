FILE(
	GLOB_RECURSE
	source_files
	*.[ch]
)

ADD_EXECUTABLE(rarcrack ${source_files})
TARGET_LINK_LIBRARIES(rarcrack ${GLIB2_LIBRARIES})
