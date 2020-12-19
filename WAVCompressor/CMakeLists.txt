cmake_minimum_required(VERSION 3.0.0)
project(FFTWavProcessing VERSION 0.1.0)

include_directories(.)

add_library(FFT SHARED FFT.cpp FFT.hpp)
add_library(WAVCompressor SHARED WAVCompressor.cpp WAVCompressor.hpp)

add_executable(FFTWavProcessing main.cpp)

target_link_libraries(WAVCompressor FFT)
target_link_libraries(FFTWavProcessing FFT WAVCompressor)


