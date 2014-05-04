cmake_minimum_required (VERSION 2.6) 
find_package( Boost )

include_directories("/Users/thijs/hithub/boost" ${Boost_INCLUDE_DIRS} )


project (kat_vectors)
set( CMAKE_BUILD_TYPE "Release" )

add_executable (kat_vectors kat_vectors.cpp)
target_link_libraries(kat_vectors)
