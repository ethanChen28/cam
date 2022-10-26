cmake_minimum_required(VERSION 3.1)

project(CaptureCamerTest)

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

# add_definitions(
    
# )

include_directories(${SOLUTION_DIR}/include)
include_directories(${SOLUTION_DIR}/src)

file(GLOB RTSP_SOURCES ${SOLUTION_DIR}/src/rtsp/*.cpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/rtsp/*.cpp
                       ${SOLUTION_DIR}/src/base/*.cpp)

file(GLOB CLIENT_SOURCES ${SOLUTION_DIR}/src/net/*.cpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/net/*.cpp
                       ${SOLUTION_DIR}/src/base/*.cpp)

file(GLOB ARCTERN_SOURCES ${SOLUTION_DIR}/src/arctern/*.cpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/arctern/*.cpp
                       ${SOLUTION_DIR}/src/base/*.cpp )

file(GLOB AUTH_SOURCES ${SOLUTION_DIR}/src/auth/*.cpp
                       ${CMAKE_CURRENT_SOURCE_DIR}/auth/*.cpp
                       ${SOLUTION_DIR}/src/base/*.cpp )                        
set(AUTH_SOURCES ${AUTH_SOURCES} ${SOLUTION_DIR}/src/net/AuthProvider.cpp)
# message("RTSP_SOURCES: ${RTSP_SOURCES}")
if(WITH_RKNN)
        message("==================test rknn===================")
        add_definitions(-DWITH_RKNN)
        set(RTSP_SOURCES ${RTSP_SOURCES}  
                         ${SOLUTION_DIR}/src/rtsp/rknn/rknnFrame.cpp
                         ${SOLUTION_DIR}/3rd/rknn/RKinclude/common/sample_common_isp.cpp)

        set(ARCTERN_SOURCES ${ARCTERN_SOURCES}  
                            #${SOLUTION_DIR}/src/arctern/face/faceDetector.cpp
                            #${SOLUTION_DIR}/src/arctern/face/faceTracker.cpp
                            ${SOLUTION_DIR}/src/arctern/comm/commDetector.cpp
                            ${SOLUTION_DIR}/src/arctern/comm/commTracker.cpp
                            )
        message("RTSP_SOURCES:　${RTSP_SOURCES}")
        message("CLIENT_SOURCES:　${CLIENT_SOURCES}")
        message("ARCTERN_SOURCES:　${ARCTERN_SOURCES}")
        message("PLATFORM_LIBS:　${PLATFORM_LIBS}")
        message("COMMON_LIBS:　${COMMON_LIBS}")
        message("OPENCV_LIBS:　${OPENCV_LIBS}")
endif()

add_executable(${PROJECT_NAME}-rtsp ${RTSP_SOURCES})

target_link_libraries(${PROJECT_NAME}-rtsp ${PLATFORM_LIBS} 
                                           ${COMMON_LIBS}
                                           ${OPENCV_LIBS}
)


add_executable(${PROJECT_NAME}-arctern ${ARCTERN_SOURCES})

target_link_libraries(${PROJECT_NAME}-arctern ${PLATFORM_LIBS} 
                                              ${COMMON_LIBS}
                                              ${OPENCV_LIBS}
)



add_executable(${PROJECT_NAME}-client ${CLIENT_SOURCES})

target_link_libraries(${PROJECT_NAME}-client ${OATPP_LIBS} 
                                             ${COMMON_LIBS}
)

add_executable(${PROJECT_NAME}-auth ${AUTH_SOURCES})

target_link_libraries(${PROJECT_NAME}-auth ${OATPP_LIBS}
                                           ${OPENSSL_LIBS}
                                           ${COMMON_LIBS}                  
)