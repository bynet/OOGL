macro(qt5_copy_dll APP DLL)
  # find the release *.dll file
  get_target_property(Qt5_${DLL}Location Qt5::${DLL} LOCATION)
  # find the debug *d.dll file
  get_target_property(Qt5_${DLL}LocationDebug Qt5::${DLL} IMPORTED_LOCATION_DEBUG)

  add_custom_command(TARGET ${APP} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<$<CONFIG:Debug>:${Qt5_${DLL}LocationDebug}> $<$<NOT:$<CONFIG:Debug>>:${Qt5_${DLL}Location}> $<TARGET_FILE_DIR:${APP}>)
endmacro()

macro(qt5_copy_dlls APP DLLs)

  foreach(DLL ${DLLs})
    qt5_copy_dll(${APP} ${DLL})
  endforeach()

endmacro()

macro(copy_shaders)

  foreach(FILE ${ARGN})

    # add_custom_command(
    #   TARGET ${OUTPUT} PRE_BUILD
    #   COMMAND ${CMAKE_COMMAND} -E copy_if_different
    #   ${CMAKE_CURRENT_SOURCE_DIR}/${FILE}
    #   ${CMAKE_CURRENT_BINARY_DIR}/${FILE})

    # configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${FILE} ${CMAKE_CURRENT_BINARY_DIR}/${FILE} COPYONLY )

    # execute_process(COMMAND ${CMAKE_COMMAND}  -E  copy_if_different
    #   ${CMAKE_CURRENT_SOURCE_DIR}/${FILE}
    #   ${CMAKE_CURRENT_BINARY_DIR}/${FILE}) 

    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/${FILE} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    
  endforeach()

endmacro()


macro(parse_argument_list PREFIX KEYWORDS ARGUMENTS)
  # Indirection over KEYWORD_LIST necessary because macro parameters are no real variables.

  set(KEYWORD_LIST "${KEYWORDS}")
  set(CURRENT_KEYWORD "")

  # Parse argument list
  foreach(ARG ${ARGUMENTS})
    # See if current argument is in the list of keywords
    list(FIND KEYWORD_LIST "${ARG}" KEYWORD_FOUND)
    if(NOT KEYWORD_FOUND EQUAL -1)
      # If current element is a keyword, store it
      set(CURRENT_KEYWORD ${ARG})
    else()
      # Append current argument to last stored keyword variable
      set(${PREFIX}_${CURRENT_KEYWORD} ${${PREFIX}_${CURRENT_KEYWORD}};${ARG})
    endif()
  endforeach()
endmacro()


macro(build_exe MODULE_NAME)

  parse_argument_list("MODULE" "SOURCES;HEADERS;RESOURCES;LIBS;FORMS" "${ARGN}")

  project(${MODULE_NAME})
  set(MODULE_DIR "${PROJECT_SOURCE_DIR}")


  if(EXISTS "${MODULE_DIR}/config.h.in")
    configure_file ("${MODULE_DIR}/config.h.in"	"${PROJECT_BINARY_DIR}/config.h")
  endif()


  include_directories("${PROJECT_BINARY_DIR}")


  if(MODULE_FORMS)
    string(REPLACE " " ";" ${MODULE_FORMS} ${MODULE_FORMS})
    foreach(FORM ${MODULE_FORMS})
      qt5_wrap_ui(UI_HEADERS ${MODULE_DIR}/ui_forms/${FORM})
    endforeach()
  endif()

  if(MODULE_RESOURCES)
    string(REPLACE " " ";" ${MODULE_RESOURCES} ${MODULE_RESOURCES})
    foreach(RESOURCE ${MODULE_RESOURCES})
      qt5_add_resources(RCC_SOURCES ${MODULE_DIR}/resources/${RESOURCE})
    endforeach()
  endif()


  if(MODULE_HEADERS)
    string(REPLACE " " ";" ${MODULE_HEADERS} ${MODULE_HEADERS})
    foreach(INC ${MODULE_HEADERS})
      include_directories(${INC})
    endforeach()
  endif()


  add_executable(${PROJECT_NAME}  ${MODULE_SOURCES} ${MODULE_HEADERS} ${UI_HEADERS} ${RCC_SOURCES})
  set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${MODULE_NAME})


  if(MODULE_LIBS)
    string(REPLACE " " ":" ${MODULE_LIBS} ${MODULE_LIBS})
    foreach(LIB ${MODULE_LIBS})
      target_link_libraries(${PROJECT_NAME}  "${LIB}")
    endforeach()
  endif()


endmacro()

macro(build_win32_exe MODULE_NAME)

  parse_argument_list("MODULE" "SOURCES;HEADERS;RESOURCES;LIBS;FORMS" "${ARGN}")

  project(${MODULE_NAME})
  set(MODULE_DIR "${PROJECT_SOURCE_DIR}")


  if(EXISTS "${MODULE_DIR}/config.h.in")
    configure_file ("${MODULE_DIR}/config.h.in"	"${PROJECT_BINARY_DIR}/config.h")
  endif()


  include_directories("${PROJECT_BINARY_DIR}")


  if(MODULE_FORMS)
    string(REPLACE " " ";" ${MODULE_FORMS} ${MODULE_FORMS})
    foreach(FORM ${MODULE_FORMS})
      qt5_wrap_ui(UI_HEADERS ${MODULE_DIR}/ui_forms/${FORM})
    endforeach()
  endif()

  if(MODULE_RESOURCES)
    string(REPLACE " " ";" ${MODULE_RESOURCES} ${MODULE_RESOURCES})
    foreach(RESOURCE ${MODULE_RESOURCES})
      qt5_add_resources(RCC_SOURCES ${MODULE_DIR}/resources/${RESOURCE})
    endforeach()
  endif()


  if(MODULE_HEADERS)
    string(REPLACE " " ";" ${MODULE_HEADERS} ${MODULE_HEADERS})
    foreach(INC ${MODULE_HEADERS})
      include_directories(${INC})
    endforeach()
  endif()


  add_executable(${PROJECT_NAME}  WIN32 ${MODULE_SOURCES} ${MODULE_HEADERS} ${UI_HEADERS} ${RCC_SOURCES})
  set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${MODULE_NAME})


  if(MODULE_LIBS)
    string(REPLACE " " ":" ${MODULE_LIBS} ${MODULE_LIBS})
    foreach(LIB ${MODULE_LIBS})
      target_link_libraries(${PROJECT_NAME}  "${LIB}")
    endforeach()
  endif()


endmacro()


macro(build_lib MODULE_NAME)

  parse_argument_list("MODULE" "SOURCES;HEADERS;LIBS" "${ARGN}")

  project(${MODULE_NAME})
  set(MODULE_DIR "${PROJECT_SOURCE_DIR}")


  if(EXISTS "${MODULE_DIR}/libconfig.h.in")
    configure_file ("${MODULE_DIR}/libconfig.h.in"	"${PROJECT_BINARY_DIR}/libconfig.h")
  endif()

  include_directories("${PROJECT_BINARY_DIR}")

  if(MODULE_HEADERS)
    string(REPLACE " " ";" ${MODULE_HEADERS} ${MODULE_HEADERS})
    foreach(INC ${MODULE_HEADERS})
      include_directories(${INC})
    endforeach()
  endif()

  add_library(${PROJECT_NAME}  ${MODULE_SOURCES} ${MODULE_HEADERS})
  set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME ${MODULE_NAME})
#  target_link_libraries(${PROJECT_NAME} )

  if(MODULE_LIBS)

    string(REPLACE " " ";" MODULE_LIBS ${MODULE_LIBS})

    foreach(LIB ${MODULE_LIBS})
     # include_directories("../${LIB}")
	 message("_lib_ " ${LIB})
     target_link_libraries(${PROJECT_NAME} "${LIB}")
    endforeach()

  endif()

endmacro()
