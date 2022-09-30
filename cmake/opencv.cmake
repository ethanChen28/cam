set(open_include ${SOLUTION_DIR}/3rd/opencv/include/)
set(open_lib ${SOLUTION_DIR}/3rd/opencv/lib/)

include_directories(${open_include})
link_directories(${open_lib})

set(OPENCV_LIBS -Wl,--start-group
                ${open_lib}/libIlmImf.a
                ${open_lib}/liblibjasper.a
                ${open_lib}/liblibjpeg.a
                ${open_lib}/liblibpng.a
                ${open_lib}/liblibtiff.a
                ${open_lib}/liblibwebp.a
                ${open_lib}/libopencv_calib3d.a
                ${open_lib}/libopencv_core.a
                ${open_lib}/libopencv_features2d.a
                ${open_lib}/libopencv_flann.a
                ${open_lib}/libopencv_highgui.a
                ${open_lib}/libopencv_imgcodecs.a
                ${open_lib}/libopencv_imgproc.a
                ${open_lib}/libopencv_ml.a
                ${open_lib}/libopencv_objdetect.a
                ${open_lib}/libopencv_photo.a
                ${open_lib}/libopencv_shape.a
                ${open_lib}/libopencv_stitching.a
                ${open_lib}/libopencv_superres.a
                ${open_lib}/libopencv_ts.a
                ${open_lib}/libopencv_video.a
                ${open_lib}/libopencv_videoio.a
                ${open_lib}/libopencv_videostab.a
                ${open_lib}/libzlib.a
                -Wl,--end-group
)