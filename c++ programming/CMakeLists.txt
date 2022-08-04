project(shad-cpp0)

cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD             17)
set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

find_package(Catch REQUIRED)
find_package(PNG)
find_package(JPEG)

find_package(Poco QUIET COMPONENTS Foundation Net JSON)
if (NOT Poco_FOUND)
  message(STATUS "Seems like POCO is not installed on your machine.")
  message(STATUS "Tasks that depend on POCO are disabled.")
endif()

find_package(PythonLibs 3 QUIET)
if (NOT PythonLibs_FOUND)
  message(STATUS "python3 libraries not found. Python tasks are disabled.")
endif()

include(cmake/TestSolution.cmake)
include(cmake/BuildFlags.cmake)
include_directories(util)

add_subdirectory(multiplication)

add_subdirectory(bin-pow)
add_subdirectory(range)
add_subdirectory(unique)
add_subdirectory(split)
add_subdirectory(polish-notation)
add_subdirectory(word-count)
add_subdirectory(permutations)
add_subdirectory(reverse-map)
add_subdirectory(long-sum)
add_subdirectory(diff-pairs)

add_subdirectory(quadratic)
add_subdirectory(pt-triangle)
add_subdirectory(factorization)
add_subdirectory(rotate)
add_subdirectory(sort-students)
add_subdirectory(war)
add_subdirectory(filter-even)
add_subdirectory(swap-sort)
add_subdirectory(entrance)
add_subdirectory(decoder)

add_subdirectory(stack)
add_subdirectory(ring-buffer)
add_subdirectory(matrix)
add_subdirectory(static-map)
add_subdirectory(string-view)
add_subdirectory(dungeon)

add_subdirectory(phone-book)

add_subdirectory(vector)
add_subdirectory(lru-cache)
add_subdirectory(cow-vector)

add_subdirectory(dedup)
add_subdirectory(smart-ptr)
add_subdirectory(deque)

add_subdirectory(find-last)
add_subdirectory(local-max)
add_subdirectory(partition)
add_subdirectory(fold)
add_subdirectory(functors)
add_subdirectory(itertools)
add_subdirectory(matrix-2.0)

add_subdirectory(list)
add_subdirectory(calculator)
add_subdirectory(reader)

add_subdirectory(safe-transform)
add_subdirectory(intrusive-list)
add_subdirectory(any)
add_subdirectory(tryhard)

add_subdirectory(pimpl)

add_subdirectory(editor)
add_subdirectory(small-test-framework)
add_subdirectory(fast-dither)

add_subdirectory(grep)
add_subdirectory(news-aggregator)

add_subdirectory(scala-vector)
add_subdirectory(matrix-3.0)

add_subdirectory(scheme)
add_subdirectory(scheme-tokenizer)
add_subdirectory(scheme-parser)

if (Poco_FOUND)
  add_subdirectory(hello-http)
  add_subdirectory(bot)
endif()

if (PythonLibs_FOUND)
  add_subdirectory(hello-python)
  add_subdirectory(numpy)
endif()

add_subdirectory(regexp)

add_subdirectory(string-operations)

if (${PNG_FOUND} AND ${JPEG_FOUND})
  add_subdirectory(raytracer-debug)
  add_subdirectory(raytracer-geom)
  add_subdirectory(raytracer-reader)
  add_subdirectory(raytracer)
  add_subdirectory(raytracer-b2)
elseif()
  message(STATUS "raytracer disabled. PNG:${PNG_FOUND} JPEG:${JPEG_FOUND}")
endif()
