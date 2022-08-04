if (TEST_SOLUTION)
  add_library(libscheme
    ../private/scheme/parser.cpp
    ../private/scheme/scheme.cpp)

  target_include_directories(libscheme
    PUBLIC ../private/scheme)
else()
  # Add your .cpp files to this library.
  add_library(libscheme
    parser.cpp
    scheme.cpp
    object.cpp)

  target_include_directories(libscheme PUBLIC .)
endif()

# scheme-repl is not used for testing, but you may use it if you need to experiment with the code.
add_executable(scheme-repl main.cpp)
target_link_libraries(scheme-repl libscheme)

add_catch(test_scheme
  test/test_boolean.cpp
  test/test_control_flow.cpp
  test/test_eval.cpp
  test/test_integer.cpp
  test/test_lambda.cpp
  test/test_list.cpp
  test/test_symbol.cpp
  SOLUTION_SRCS test/scheme_test.cpp)

target_link_libraries(test_scheme libscheme)
