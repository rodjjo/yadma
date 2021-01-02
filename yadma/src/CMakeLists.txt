file(GLOB Sources
    "${CMAKE_CURRENT_SOURCE_DIR}/autenticator/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/database/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/download/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/settings/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/thread/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/pages/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/webserver/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    # "${CMAKE_CURRENT_SOURCE_DIR}/../third-party/mongoose/src/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../third-party/mongoose/mongoose.c"
    )

if(WIN32)
add_executable(yadma WIN32 ${Sources})
else(WIN32)
add_executable(yadma ${Sources})
endif(WIN32)

add_dependencies(yadma CURL)
add_dependencies(yadma SQLITE)

if(WIN32)
target_link_libraries(yadma
                      ${CMAKE_THREAD_LIBS_INIT}
                      "sqlite3"
                      "curl_imp"
                      "ws2_32"
                      ${CMAKE_DL_LIBS}
                    )
else(WIN32)
target_link_libraries(yadma
                      ${CMAKE_THREAD_LIBS_INIT}
                      "sqlite3"
                      "curl"
                      ${CMAKE_DL_LIBS}
                    )
endif(WIN32)


if(WIN32)
else(WIN32)
SET_TARGET_PROPERTIES(yadma PROPERTIES LINKER_LANGUAGE C)
endif(WIN32)

install(TARGETS yadma
        RUNTIME DESTINATION bin
        COMPONENT runtime)

