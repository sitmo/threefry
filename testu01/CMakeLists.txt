cmake_minimum_required (VERSION 2.6) 
find_package( Boost )

include_directories("/Users/thijs/hithub/boost" ${Boost_INCLUDE_DIRS} )


project (testu01)
set( CMAKE_BUILD_TYPE "Release" )

add_executable (threefry_testu01 threefry_testu01.cpp)
target_link_libraries(threefry_testu01 testu01)

