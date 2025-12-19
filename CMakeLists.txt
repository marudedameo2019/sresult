cmake_minimum_required(VERSION 3.10)
project(sresult CXX)

if(MSVC)
    add_compile_options(/source-charset:utf-8 /execution-charset:.932 /std:c++17)
else()
    add_compile_options(-fexec-charset=cp932 -std=c++17)
    # clang用
    # add_compile_options(-std=c++17)
    add_link_options(-static)
endif()

add_executable(railway_lambda               railway_lambda.cpp)
add_executable(railway_function             railway_function.cpp)
add_executable(no_railway_exception         no_railway_exception.cpp)
add_executable(railway_lambda_with_myvector railway_lambda_with_myvector.cpp)
add_executable(railway_lambda_bench         railway_lambda_bench.cpp)