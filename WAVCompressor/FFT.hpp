//
//  FFT.hpp
//  FFTWavProcessing
//
//  Created by Артем Андрианов on 14.12.2020.
//

#ifndef FFT_h
#define FFT_h

#include <iostream>
#include <complex>
#include <vector>
#include <algorithm>
#include <math.h>

using cld = std::complex <long double>;

size_t FindUpperDegreeOfTwo( size_t v );

std::vector <cld> MakeComplexVector( const std::vector<int> & a, size_t n );

std::vector <cld> MakeGeneralFFT(std::vector <cld> a, cld q);

std::vector <cld> MakeFFT(const std::vector <cld> & a);

std::vector <cld> MakeInverseFFT(std::vector <cld> a);

std::vector <int> MakeIntVector(const std::vector<cld> & a);

std::vector <int>  MultiplicatePolynoms( const std::vector <int> & a, const std::vector <int> & b );

std::vector <int> ReadVector();

#endif /* FFT_h */
