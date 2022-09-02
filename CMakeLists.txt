cmake_minimum_required(VERSION 3.23)
project(RayTracing)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wpedantic -Wextra -Wconversion" )
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -flto" )

add_executable(RayTracing main.cpp
        src/Scene.h src/HitRecord.h src/Material.h src/Camera.h src/Random.h src/Definitions.h src/MaterialData.h src/AABB.h src/BoundingVolumeHierarchy.h src/Rectangle.h)

find_package(Threads REQUIRED)
find_package(TBB REQUIRED)
target_link_libraries(RayTracing
        PRIVATE Threads::Threads
        PRIVATE TBB::tbb)