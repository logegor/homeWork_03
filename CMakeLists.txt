cmake_minimum_required(VERSION 3.10)

set(PATCH_VERSION "2" CACHE INTERNAL "Patch version")
set(PROJECT_VESRION 0.1.${PATCH_VERSION})

project(homework VERSION ${PROJECT_VESRION})

find_package(GTest REQUIRED)

configure_file(version.h.in version.h)

add_executable(home_work my_allocator.cpp)
add_library(version lib.cpp)
add_executable(test_version test_version.cpp)

set_target_properties(home_work version test_version PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
)
target_include_directories(version
    PRIVATE "${CMAKE_BINARY_DIR}"
)

target_link_libraries(home_work PRIVATE
    version
)
target_link_libraries(test_version
    GTest::gtest_main
    version
)

if (MSVC)
    target_compile_options(home_work PRIVATE
        /W4
    )
    target_compile_options(version PRIVATE
        /W4
    )
    target_compile_options(test_version PRIVATE
        /W4
    )
else ()
    target_compile_options(home_work PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(version PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(test_version PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()



install(TARGETS home_work RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

include(CPack)

enable_testing()

add_test(test_version test_version)
