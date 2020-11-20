#include <iostream>
#include <vector>

const double EPSILON = 0.0001;

bool areEqual(const double& first, const double& second) {
    return abs(first - second) <= EPSILON;
}

class CPoint {
public:
    CPoint();
    CPoint(long long x, long long y);
    CPoint(const CPoint&);
    
    CPoint& operator=(const CPoint&);
    
    friend CPoint& operator+=(CPoint&, const CPoint&);
    friend CPoint operator+(const CPoint&, const CPoint&);
    
    friend CPoint& operator-=(CPoint&, const CPoint&);
    friend CPoint operator-(const CPoint&, const CPoint&);

    friend void PrintPoint(const CPoint&);
    
    long long x;
    long long y;
};

CPoint::CPoint() : x(0), y(0) {};
CPoint::CPoint(long long x, long long y) : x(x), y(y) {};
CPoint::CPoint(const CPoint& rhs) : x(rhs.x), y(rhs.y) {};

CPoint& CPoint::operator=(const CPoint& rhs) {
    x = rhs.x;
    y = rhs.y;
    return (*this);
}

CPoint& operator+=(CPoint& lhs, const CPoint& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

CPoint operator+(const CPoint& lhs, const CPoint& rhs) {
    CPoint result = lhs;
    result += rhs;
    return result;
}

CPoint& operator-=(CPoint& lhs, const CPoint& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

CPoint operator-(const CPoint& lhs, const CPoint& rhs) {
    CPoint result = lhs;
    result -= rhs;
    return result;
}

void PrintPoint(const CPoint& point) {
    std::cout << point.x << " " << point.y << std::endl;
}


long long OrientedArea (const CPoint& first, const CPoint& second, const CPoint& third) {
    return (second.x - first.x) * (third.y - first.y) - (second.y - first.y) * (third.x - first.x);
}
 
bool AreProjectionsIntersect (long long firstStart, long long  firstEnd, long long  secondStart, long long  secondEnd) {
    if (firstStart > firstEnd) {
        std::swap (firstStart, firstEnd);
    }
    if (secondStart > secondEnd)  {
        std::swap (secondStart, secondEnd);
    }
    return std::max(firstStart, secondStart) <= std::min(firstEnd, secondEnd);
}
 
bool AreSegmentsCross (const CPoint& firstStart, const CPoint& firstEnd, const CPoint& secondStart, const CPoint& secondEnd) {
    return AreProjectionsIntersect (firstStart.x, firstEnd.x, secondStart.x, secondEnd.x)
        && AreProjectionsIntersect (firstStart.y, firstEnd.y, secondStart.y, secondEnd.y)
        && OrientedArea(firstStart, firstEnd, secondStart) * OrientedArea(firstStart, firstEnd, secondEnd) <= 0
        && OrientedArea(secondStart, secondEnd, firstStart) * OrientedArea(secondStart, secondEnd, firstEnd) <= 0;
}

int main() {
    CPoint A;
    CPoint B;
    std::cin >> A.x >> A.y;
    std::cin >> B.x >> B.y;
    int N;
    std::cin >> N;
    int cnt = 0;
    for (int i = 0; i < N; ++i) {
        CPoint start, end;
        std::cin >> start.x >> start.y;
        std::cin >> end.x >> end.y;
        if (AreSegmentsCross(A, B, start, end)) {
            ++cnt;
        }
    }
    std::cout << cnt;
    return 0;
}
