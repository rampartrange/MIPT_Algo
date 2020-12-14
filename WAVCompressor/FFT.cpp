#include "FFT.hpp"

size_t FindUpperDegreeOfTwo( size_t v )
{
    size_t n = 1;
    while ( n < v ) {
        n *= 2;
    }
    return n * 2;
}

std::vector <cld> MakeComplexVector( const std::vector<int> & a, size_t n )
{
    std::vector <cld> ca( n, cld( 0.0, 0.0 ) );
    for ( size_t i = 0; i < a.size(); i++ ) {
        ca[i] = cld( a[i], 0.0 );
    }
    return ca;
}

std::vector <cld> MakeGeneralFFT(std::vector <cld> a, cld q)
{
    if (a.size() == 1)
        return a;
    std::vector <cld> a0, a1;
    for (size_t i = 0; i < a.size(); i += 2) {
        a0.push_back(a[i]);
        a1.push_back(a[i + 1]);
    }
    
    a0 = MakeGeneralFFT(a0, q * q);
    a1 = MakeGeneralFFT(a1, q * q);

    cld w (1.0, 0.0);
    
    for (size_t i = 0; i < a.size() / 2; i++) {
        cld u = a0[i];
        cld v = w * a1[i];
        a[i] = u + v;
        a[i + a.size() / 2] = u - v;
        w *= q;
    }
    
    return a;
}

std::vector <cld> MakeFFT(const std::vector <cld> & a)
{
    long double ang = 2.0 * M_PI / a.size();
    return MakeGeneralFFT(a, cld(std::cosl(ang), std::sinl(ang)));
}

std::vector <cld> MakeInverseFFT(std::vector <cld> a)
{
    long double ang = 2.0 * M_PI / a.size();
    a = MakeGeneralFFT(a, cld(std::cosl(ang), -std::sinl(ang)));
    for (size_t i = 0; i < a.size(); i++) {
        a[i] /= a.size();
    }
    return a;
}

std::vector <int> MakeIntVector(const std::vector<cld> & a)
{
    std::vector <int> ans(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        ans[i] = floorl(a[i].real() + 0.5);
    }
    return ans;
}

std::vector <int>  MultiplicatePolynoms( const std::vector <int> & a, const std::vector <int> & b )
{
    size_t n = FindUpperDegreeOfTwo( std::max(a.size(), b.size()) );
    
    std::vector <cld> ca = MakeComplexVector(a, n);
    std::vector <cld> cb = MakeComplexVector(b, n);
    
    ca = MakeFFT(ca);
    cb = MakeFFT(cb);
    
    for ( size_t i = 0; i < n; i++ ) {
        ca[i] *= cb[i];
    }
    
    std::vector <cld> cc = MakeInverseFFT(ca);
    
    return MakeIntVector(cc);
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



