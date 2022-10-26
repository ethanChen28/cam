add_definitions(-DWITH_RKNN)
# add_definitions(-DRKAIQ)
add_definitions(-mfpu=neon-vfpv4 -ffast-math -mfloat-abi=hard -fopenmp)

set(rknn_include ${SOLUTION_DIR}/3rd/rknn/RKinclude)
set(rknn_lib ${SOLUTION_DIR}/3rd/rknn/RKlib)

include_directories(${rknn_include})
include_directories(${rknn_include}/common)
include_directories(${rknn_include}/include)
# include_directories(${rknn_include}/include/common)
include_directories(${rknn_include}/include/rkmedia)

link_directories(${rknn_lib})

set(PLATFORM_LIBS ${PLATFORM_LIBS}  
                    ${rknn_lib}/libeasymedia.so
                    ${rknn_lib}/librockface.so
                    ${rknn_lib}/libliveMedia.so
                    ${rknn_lib}/librknn_api.so
                    # ${rknn_lib}/libz.so
                    ${rknn_lib}/libdrm.so
                    ${rknn_lib}/librockchip_mpp.so
                    ${rknn_lib}/libBasicUsageEnvironment.so
                    ${rknn_lib}/libgroupsock.so
                    ${rknn_lib}/libRKAP_3A.so
                    ${rknn_lib}/libUsageEnvironment.so
                    ${rknn_lib}/libRKAP_Common.so
                    ${rknn_lib}/libv4l2.so
                    ${rknn_lib}/libsqlite3.so
                    ${rknn_lib}/libmd_share.so
                    ${rknn_lib}/libod_share.so
                    ${rknn_lib}/librkaiq.so
                    ${rknn_lib}/libasound.so
                    ${rknn_lib}/libv4lconvert.so
                    ${rknn_lib}/libRKAP_ANR.so
                    ${rknn_lib}/libthird_media.so
                    ${rknn_lib}/librga.so
)

set(arctern_include ${SOLUTION_DIR}/3rd/rknn/arctern/include/)
set(arctern_lib ${SOLUTION_DIR}/3rd/rknn/arctern/lib)

include_directories(${arctern_include})
link_directories(${arctern_lib})

set(PLATFORM_LIBS ${PLATFORM_LIBS} ${arctern_lib}/libarctern-base.so
)

include_directories(/usr/local/arctern/include)
link_directories(/usr/local/arctern/lib)

include_directories(/usr/local/include/eigen3/)
# find_package(Eigen3)

set(PLATFORM_LIBS ${PLATFORM_LIBS} ArcternAlg
)

set(COMMON_LIBS ${COMMON_LIBS} uuid dl rt pthread       
)

include_directories(/usr/local/include)
link_directories(/usr/local/lib)
set(OPENSSL_LIBS ssl crypto)