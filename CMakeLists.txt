cmake_minimum_required (VERSION 3.19.1)
project(ImageServer)

include(ExternalProject)

set(CMAKE_CXX_STANDARD 17)

set(LIBCOUCHBASE_DEST ${CMAKE_CURRENT_SOURCE_DIR}/build/couchbase)
set(LIBCOUCHBASE_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libcouchbase)

ExternalProject_Add(libcouchbase
    SOURCE_DIR ${LIBCOUCHBASE_SOURCE}
    BINARY_DIR libcouchbase-build
    INSTALL_DIR ${LIBCOUCHBASE_DEST}
    CONFIGURE_COMMAND ${LIBCOUCHBASE_SOURCE}/cmake/configure -prefix ${LIBCOUCHBASE_DEST}
    
    BUILD_COMMAND make 
)


set(LIBMICROHTTPD_DEST ${CMAKE_CURRENT_SOURCE_DIR}/build/microhttpd)
set(LIBMICROHTTPD_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libmicrohttpd)

ExternalProject_Add(libmicrohttpd
    SOURCE_DIR ${LIBMICROHTTPD_SOURCE}
    BINARY_DIR libmicrohttpd-build
    INSTALL_DIR ${LIBMICROHTTPD_DEST}
    CONFIGURE_COMMAND ${LIBMICROHTTPD_SOURCE}/configure -prefix ${LIBMICROHTTPD_DEST}
    
    BUILD_COMMAND make 
)

ExternalProject_Add_Step(libmicrohttpd touch-files
    COMMAND touch aclocal.m4 configure Makefile.am Makefile.in
    WORKING_DIRECTORY ${LIBMICROHTTPD_SOURCE}
    LOG 1
    DEPENDERS configure
)

set(LIBHTTPSERVER_DEST ${CMAKE_CURRENT_SOURCE_DIR}/build/httpserver)
set(LIBHTTPSERVER_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/lib/libhttpserver)

ExternalProject_Add(libhttpserver
    SOURCE_DIR ${LIBHTTPSERVER_SOURCE}
    BINARY_DIR libhttpserver-build
    INSTALL_DIR ${LIBHTTPSERVER_DEST}
    CONFIGURE_COMMAND ${LIBHTTPSERVER_SOURCE}/configure -prefix ${LIBHTTPSERVER_DEST} CXXFLAGS=-I${LIBMICROHTTPD_DEST}/include LDFLAGS=-L${LIBMICROHTTPD_DEST}/lib
    
    BUILD_COMMAND make 
)

ExternalProject_Add_Step(libhttpserver bootstrap
    COMMAND ./bootstrap
    WORKING_DIRECTORY ${LIBHTTPSERVER_SOURCE}
    LOG 1
    DEPENDERS configure
)

set(MAGICK_DEST ${CMAKE_CURRENT_SOURCE_DIR}/build/ImageMagick)
set(MAGICK_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/lib/ImageMagick)

ExternalProject_Add(ImageMagick
    SOURCE_DIR ${MAGICK_SOURCE}
    BINARY_DIR ImageMagick-build
    INSTALL_DIR ${MAGICK_DEST}
    CONFIGURE_COMMAND ${MAGICK_SOURCE}/configure -prefix ${MAGICK_DEST} --with-jpeg=yes --with-png=yes --with-webp=yes
    
    BUILD_COMMAND make 
)

set(OPENCV_DEST ${CMAKE_CURRENT_SOURCE_DIR}/build/opencv)
set(OPENCV_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/lib/opencv)

ExternalProject_Add(OpenCV
    SOURCE_DIR ${OPENCV_SOURCE}
    BINARY_DIR opencv-build
    INSTALL_DIR ${OPENCV_DEST}
    CONFIGURE_COMMAND cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DBUILD_PNG=ON -DBUILD_JPEG=ON -DCMAKE_INSTALL_PREFIX=${OPENCV_DEST} ${OPENCV_SOURCE}
    BUILD_COMMAND make
)

set(SRC src/main.cpp
        src/App.cpp
        src/Config.cpp
        src/utils/hex.cpp
        src/utils/md5.cpp
        src/utils/MultiPartForm.cpp
        src/utils/ThreadPool.cpp
        src/utils/Socket.cpp
        src/utils/ImageResponse.cpp
        src/utils/mime.cpp
        src/utils/convert.cpp
        src/utils/base64.cpp
        src/model/Image.cpp
        src/model/ImageVersion.cpp
        src/model/Sprite.cpp
        src/model/Rect.cpp
        src/repos/ImageStore.cpp
		src/couchbase/CreateOpts.cpp
		src/couchbase/Connection.cpp
		src/couchbase/Command.cpp
		src/couchbase/Read.cpp
		src/couchbase/Write.cpp
		src/couchbase/Worker.cpp
        src/couchbase/Config.cpp
        src/couchbase/Entry.cpp
        src/couchbase/ReadMulti.cpp
        src/couchbase/WriteMulti.cpp
        src/renderer/ImageRenderer.cpp
        src/renderer/OpenCVRenderer.cpp
        src/renderer/MagickRenderer.cpp
        src/sprite_controller/SpriteController.cpp
        src/image_controller/ImageController.cpp
		src/services/core/Core.cpp
		src/services/core/endpoints/ImageGetter.cpp
		src/services/core/endpoints/ImageMeta.cpp
		src/services/core/endpoints/SpriteMaker.cpp
		src/services/core/endpoints/SpriteSheet.cpp
		src/services/core/endpoints/SpriteCSS.cpp
		src/services/core/endpoints/Base64Images.cpp
		src/services/uploads/Uploads.cpp
		src/services/uploads/endpoints/ImageUpload.cpp)

add_executable(image_server ${SRC})

add_dependencies(libhttpserver libmicrohttpd)
add_dependencies(image_server libhttpserver libcouchbase ImageMagick OpenCV)

set_target_properties(image_server PROPERTIES CXX_STANDARD 17)
set_target_properties(image_server PROPERTIES CXX_STANDARD_REQUIRED ON)
set_target_properties(image_server PROPERTIES CXX_EXTENSIONS OFF)
										     
set(OPENCV_ROOT lib/opencv)
set(OPENCV_MODULES ${OPENCV_ROOT}/modules)										     
set(OPENCV_INCLUDE_DIRS ${OPENCV_DEST}/include/opencv4)
                        
set(OPENCV_LIBRARY_DIRS ${OPENCV_DEST}/lib 
                        ${OPENCV_DEST}/lib64)
set(OPENCV_LIBRARIES opencv_core
                     opencv_imgcodecs
                     opencv_imgproc
                     opencv_gapi)
                        										     
target_include_directories(image_server PRIVATE 
                                    src
                                    ${LIBCOUCHBASE_DEST}/include
                                    ${LIBHTTPSERVER_DEST}/include
                                    ${LIBMICROHTTPD_DEST}/include
                                    ${MAGICK_DEST}/include/ImageMagick-7
                                    ${OPENCV_INCLUDE_DIRS})
                                                                                 
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP jsoncpp)

message("Microhttpd includes: ${MICROHTTPD_INCLUDE_DIRS}")
message("Jsoncpp libraries: ${JSONCPP_LIBRARY_DIRS}")
                                    
target_link_directories(image_server PRIVATE ${LIBHTTPSERVER_DEST}/lib
                                             ${LIBHTTPSERVER_DEST}/lib64
                                             ${LIBCOUCHBASE_DEST}/lib
                                             ${LIBCOUCHBASE_DEST}/lib64
                                             ${LIBMICROHTTPD_DEST}/lib
                                             ${LIBMICROHTTPD_DEST}/lib64
                                             ${MAGICK_DEST}/lib
                                             ${MAGICK_DEST}/lib64
                                             ${JSONCPP_LIBRARY_DIRS}
                                             ${OPENCV_LIBRARY_DIRS})
                                    
set(MAGICK_LIB Magick++-7.Q16HDRI)

if (APPLE)

    set(SSL_ROOT /usr/local/Cellar/openssl@1.1/1.1.1i)

    include_directories(BEFORE ${SSL_ROOT}/include 
                        BEFORE ${JSONCPP_INCLUDE_DIRS}
                        BEFORE ${MICROHTTPD_INCLUDE_DIRS}) 

    find_library(OPENSSL_LIB ssl PATHS ${SSL_ROOT}/lib NO_DEFAULT_PATH) 
    find_library(CRYPTO_LIB crypto PATHS ${SSL_ROOT}/lib NO_DEFAULT_PATH)
    
    message("OpenSSL: ${OPENSSL_LIB}")
    message("Crypto: ${CRYPTO_LIB}")
    
    target_link_libraries(image_server PRIVATE 
                                       couchbase
                                       ${MAGICK_LIB}
                                       ${JSONCPP_LIBRARIES}
                                       httpserver.a 
                                       ${OPENSSL_LIB}
                                       ${CRYPTO_LIB}
                                       ${OPENCV_LIBRARIES})
                                       
else()
    message("Linking for non-Apple platform")
    
    find_package(OpenSSL REQUIRED)
    
    include_directories(BEFORE ${JSONCPP_INCLUDE_DIRS}
                        BEFORE ${MICROHTTPD_INCLUDE_DIRS})
     
    target_link_libraries(image_server PRIVATE 
                                       couchbase
                                       ${MAGICK_LIB} 
                                       ${JSONCPP_LIBRARIES}
                                       httpserver.a 
                                       ssl
                                       crypto
                                       pthread
                                       microhttpd
                                       uuid
                                       ${OPENCV_LIBRARIES})
                                       
endif()

#find_package(OpenSSL REQUIRED)
										
