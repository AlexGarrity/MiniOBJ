
set (TEST_NAME ${PROJECT_NAME}TestGTest)

file(GLOB_RECURSE TEST_SRC
    Main.cpp
)

add_executable(${TEST_NAME} ${TEST_SRC})

target_link_libraries(${TEST_NAME} libgtest libgmock) 

gtest_discover_tests(${TEST_NAME})