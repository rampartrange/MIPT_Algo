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

size_t FindUpperDegreeOfTwo( size_t);

std::vector <cld> MakeComplexVector( const std::vector<int> &, size_t n);

void MakeGeneralFFT(std::vector <cld>&, cld);

void MakeFFT(std::vector <cld>&);

void MakeInverseFFT(std::vector <cld>&);

std::vector <int> MakeIntVector(const std::vector<cld>&);

std::vector <int> ReadVector();

#endif /* FFT_h */
