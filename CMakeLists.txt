cmake_minimum_required(VERSION 3.11)

project(
    Sagal
    VERSION 0.1
    DESCRIPTION "Example"
    LANGUAGES CXX
)

add_executable(i2c_set ./src/main.cpp ./src/i2c_controller.cpp ./src/pca9685.cpp)