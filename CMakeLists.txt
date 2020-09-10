cmake_minimum_required(VERSION 3.17)
project(SocketServer)

set(CMAKE_CXX_STANDARD 14)

set(SRC
    main.cpp
    src/client/client.cpp
    src/server/server.cpp
    src/socket.cpp
    src/logger/logger.cpp
    )

set(HEADERS
    src/client/client.h
    src/server/server.h
    src/server/no_numbers_exception.h
    src/server/exit_message_exception.h
    src/socket.h
    src/logger/logger.h
    )

add_executable(${PROJECT_NAME}
               ${SRC}
               ${HEADERS}
               )

# Tests
include(GoogleTest)

set(TEST_SRC
    tests/server/server_tests.cpp
    tests/server/server_tests.h
    )

gtest_add_tests(
        TARGET ${PROJECT_NAME}
        TEST_SUFFIX .noArgs
        TEST_LIST ${TEST_SRC}
)
