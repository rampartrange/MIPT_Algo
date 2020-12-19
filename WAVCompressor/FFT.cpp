#include "FFT.hpp"

size_t FindUpperDegreeOfTwo( size_t base)
{
    size_t closestNumber = 1;
    while ( closestNumber < base ) {
        closestNumber *= 2;
    }
    return closestNumber * 2;
}

std::vector <cld> MakeComplexVector( const std::vector<int> & intVector, size_t n )
{
    std::vector <cld> complexVector( n, cld( 0.0, 0.0 ) );
    auto size = intVector.size();
    for ( size_t i = 0; i < size; i++ ) {
        complexVector[i] = cld( intVector[i], 0.0 );
    }
    return complexVector;
}

void MakeGeneralFFT(std::vector <cld>& complexVector, cld shift)
{
    if (complexVector.size() == 1) {
        return;
    }
    std::vector <cld> leftSide, rightSide;
    auto size = complexVector.size();
    leftSide.reserve(size);
    rightSide.reserve(size);
    for (size_t i = 0; i < size; i += 2) {
        leftSide.push_back(complexVector[i]);
        rightSide.push_back(complexVector[i + 1]);
    }
    
    MakeGeneralFFT(leftSide, shift * shift);
    MakeGeneralFFT(rightSide, shift * shift);

    cld w (1.0, 0.0);
    
    for (size_t i = 0; i < size / 2; i++) {
        cld u = leftSide[i];
        cld v = w * rightSide[i];
        complexVector[i] = u + v;
        complexVector[i +size / 2] = u - v;
        w *= shift;
    }
    
}

void MakeFFT(std::vector <cld> &complexVector)
{
    long double angle = 2.0 * M_PI / complexVector.size();
    MakeGeneralFFT(complexVector, cld(std::cosl(angle), std::sinl(angle)));
}

void MakeInverseFFT(std::vector <cld>& complexVector)
{
    auto size = complexVector.size();
    long double angle = 2.0 * M_PI / size;
    MakeGeneralFFT(complexVector, cld(std::cosl(angle), -std::sinl(angle)));
    for (size_t i = 0; i < size; i++) {
        complexVector[i] /= size;
    }
}

std::vector <int> MakeIntVector(const std::vector<cld> & complexVector)
{
    auto size = complexVector.size();
    std::vector <int> ans(size);
    for (size_t i = 0; i < size; i++) {
        ans[i] = floorl(complexVector[i].real() + 0.5);
    }
    return ans;
}

std::vector <int> ReadVector()
{
    int n;
    std::cin >> n;
    std::vector <int> a(n);
    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }
    return a;
}



