set(SRC ugly.cpp)

if (TEST_SOLUTION)
  include_directories(../private/pimpl)
  set(SRC ../private/pimpl/ugly.cpp)
endif()


add_catch(test_pimpl ${SRC} good.cpp)
