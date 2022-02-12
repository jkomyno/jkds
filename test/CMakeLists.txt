include(FetchContent)

set(TESTS_EXECUTABLE "jkds_tests")

# Download Google Test
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG release-1.11.0)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

add_executable(${TESTS_EXECUTABLE}
    "${CMAKE_CURRENT_LIST_DIR}/container/disjoint_set_test.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/container/max_binary_heap_test.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/container/max_k_heap_test.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/container/max_priority_queue_binary_heap_test.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/container/min_binary_heap_test.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/container/min_k_heap_test.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/container/min_priority_queue_binary_heap_test.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/container/sparse_byte_set_test.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/functional/fmap_test.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/functional/zip_test.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/util/range_test.cpp")

target_link_libraries(${TESTS_EXECUTABLE} PRIVATE gtest_main jkds)
gtest_discover_tests(${TESTS_EXECUTABLE})
