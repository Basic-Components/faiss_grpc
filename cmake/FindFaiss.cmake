# 引入faiss
# Find gRPC include directory
find_path(FAISS_INCLUDE_DIR NAMES faiss
    /usr/local/include/faiss
)
if (FAISS_INCLUDE_DIR-NOTFOUND)
    message(FATAL_ERROR "找不到faiss的头文件位置")
endif()
message(NOTICE "faiss的头文件位置${FAISS_INCLUDE_DIR}")
mark_as_advanced(FAISS_INCLUDE_DIR)

find_library(FAISS_LIBRARY NAMES libfaiss.a 
    PATHS /usr/local/lib
    REQUIRED
)
if (FAISS_LIBRARY-NOTFOUND)
    message(FATAL_ERROR "找不到faiss的库文件位置")
endif()
message(NOTICE "faiss的库文件位置${FAISS_LIBRARY}")
mark_as_advanced(FAISS_LIBRARY)
add_library(faiss::libfaiss UNKNOWN IMPORTED)
set_target_properties(faiss::libfaiss PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${FAISS_INCLUDE_DIR}
    INTERFACE_LINK_LIBRARIES "-lgomp;-lopenblas;-lpthread"
    IMPORTED_LOCATION ${FAISS_LIBRARY}
)

include(${CMAKE_ROOT}/Modules/FindPackageHandleStandardArgs.cmake)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Faiss DEFAULT_MSG
    FAISS_LIBRARY FAISS_INCLUDE_DIR)