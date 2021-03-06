project(yadma C CXX)

include(ExternalProject)

cmake_minimum_required(VERSION 3.18.2 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build/bin")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

if ($ENV{YADMA_DEBUG})
    message("Compliling in Debug build type")
    set(CMAKE_BUILD_TYPE Debug)
else ($ENV{YADMA_DEBUG})
    message("Compliling in Release build type")
    set(CMAKE_BUILD_TYPE Release)
endif ($ENV{YADMA_DEBUG})


if(WIN32)
    set(SHARED_LIB_EXT "dll")
    set(USE_SSL_WINDOWS "-DCMAKE_USE_SCHANNEL=ON")
else(WIN32)
    set(SHARED_LIB_EXT "so")
    set(USE_SSL_WINDOWS "")
endif(WIN32)


ExternalProject_Add (CURL
    PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/build/curl"
    CMAKE_ARGS "${USE_SSL_WINDOWS}"
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/yadma/third-party/curl"
    DOWNLOAD_COMMAND "" # Disable download step
    # CONFIGURE_COMMAND "" # Disable all other steps
    # BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Add (SQLITE
    PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/build/sqlite"
    SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/yadma/third-party/sqlite"
    DOWNLOAD_COMMAND "" # Disable download step
    # CONFIGURE_COMMAND "" # Disable all other steps
    # BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

set(PLATFORM_SPECIFIC_LIBS "-lpthread")

set(SQLITE_LIBRARIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/sqlite/src/SQLITE-build")
set(CURL_LIBRARIES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/curl/src/CURL-build/lib")


include_directories(yadma
                    "${PROJECT_SOURCE_DIR}/yadma/src"
                    "${PROJECT_SOURCE_DIR}/yadma/third-party/curl/include"
                    "${PROJECT_SOURCE_DIR}/yadma/third-party/"
                    "${PROJECT_SOURCE_DIR}/yadma/third-party/mongoose"
                    "${PROJECT_SOURCE_DIR}/yadma/third-party/sqlite/src"
                    )

link_directories(yadma
                "${SQLITE_LIBRARIES_DIR}"
                "${CURL_LIBRARIES_DIR}")

add_subdirectory("yadma/src")

add_custom_command(
    TARGET CURL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_SOURCE_DIR}/build/curl/src/CURL-build/lib/libcurl.${SHARED_LIB_EXT}"
            "${CMAKE_CURRENT_SOURCE_DIR}/build/bin/libcurl.${SHARED_LIB_EXT}")

add_custom_command(TARGET CURL POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   "${CMAKE_CURRENT_SOURCE_DIR}/pub/www" "${CMAKE_CURRENT_SOURCE_DIR}/build/www")

if(WIN32)
add_custom_command(
    TARGET CURL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            "${CMAKE_CURRENT_SOURCE_DIR}/build/curl/src/CURL-build/lib/libcurl_imp.lib"
            "${CMAKE_CURRENT_SOURCE_DIR}/build/curl/src/CURL-build/lib/libcurl_imp.a")
endif(WIN32)