cmake_minimum_required(VERSION 3.1)

project(CaptureCamer)

set(CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        message("=============================${CMAKE_BUILD_TYPE}===========================")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
        set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -g -Wall")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
        message("=============================${CMAKE_BUILD_TYPE}===========================")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -o3 -Wall")
        set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -o3 -Wall")
endif()

set(SOLUTION_DIR ${CMAKE_CURRENT_SOURCE_DIR})

if(WITH_RKNN)
        message("==================rknn===================")
        add_definitions(-DWITH_RKNN)
        include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/rknn.cmake)

        set(PLATFORM_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/src/rtsp/rknn/rknnFrame.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/3rd/rknn/RKinclude/common/sample_common_isp.cpp)

        set(PLATFORM_SOURCE ${PLATFORM_SOURCE}  
                            ${CMAKE_CURRENT_SOURCE_DIR}/src/arctern/face/faceDetector.cpp
                            ${CMAKE_CURRENT_SOURCE_DIR}/src/arctern/face/faceTracker.cpp)
        
        message("PLATFORM_SOURCE:　${PLATFORM_SOURCE}")
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/oatpp.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/opencv.cmake)
# add_definitions(
    
# )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/base/*.cpp 
                  ${CMAKE_CURRENT_SOURCE_DIR}/src/net/*.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/src/arctern/*.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/src/rtsp/*.cpp
                  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
message("SOURCES:　${SOURCES}")


add_executable(${PROJECT_NAME} ${SOURCES} 
                               ${PLATFORM_SOURCE})

target_link_libraries(${PROJECT_NAME} ${ARCTERN_LIBS} 
                                      ${OATPP_LIBS} 
                                      ${PLATFORM_LIBS} 
                                      ${COMMON_LIBS}
                                      ${OPENCV_LIBS}
)

# add_subdirectory(test)