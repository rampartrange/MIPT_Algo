#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>

const long double EPSILON = 1e-5;
const int PRECISION = 6;

enum class Turn {
    LEFT,
    RIGHT,
    COLLINEAR
};

bool AreEqual(double first, double second) {
    return abs(first - second) <= EPSILON;
}

class Point {
public:
    Point();
    Point(long long x, long long y);
    Point(const Point&);
    
    Point& operator=(const Point&);

    
    long long x;
    long long y;
};

Point::Point() : x(0), y(0) {};
Point::Point(long long x, long long y) : x(x), y(y) {};
Point::Point(const Point& rhs) : x(rhs.x), y(rhs.y) {};

Point& Point::operator=(const Point& rhs) {
    x = rhs.x;
    y = rhs.y;
    return(*this);
}

bool operator ==(const Point& lhs, const Point& rhs) {
    return(lhs.x == rhs.x) &&(lhs.y == rhs.y);
}

bool operator !=(const Point& lhs, const Point& rhs) {
    return !(lhs == rhs);
}

bool operator <(const Point& lhs, const Point& rhs) {
    return  std::tie(lhs.x, lhs.y) < std::tie(rhs.x, rhs.y);
}

bool operator >(const Point& lhs, const Point& rhs) {
    return rhs < lhs;
}

bool operator <=(const Point& lhs, const Point& rhs) {
    return !(lhs > rhs);
}

bool operator >=(const Point& lhs, const Point& rhs) {
    return !(lhs < rhs);
}

long double DistanceBetweenPoints(const Point& lhs, const Point& rhs) {
    return sqrt((rhs.x - lhs.x) *(rhs.x - lhs.x) +
               (rhs.y - lhs.y) *(rhs.y - lhs.y));
}

std::istream &operator>>(std::istream& input, Point& point) {
   input >> point.x >> point.y;
   return input;
}

std::ostream &operator<<(std::ostream &output, const Point& point) {
    output << point.x << " " << point.y;
    return output;
}

class Vector {
public:
    Vector(long long, long long);
    Vector(const Point&, const Point&);
    Vector& operator=(const Vector& rhs);
    
    static long long CrossProduct(const Vector& first, const Vector& second);
    
    long long x;
    long long y;
};

Vector::Vector(long long x, long long y) : x(x), y(y) {};
Vector::Vector(const Point& lhs, const Point& rhs) : x(rhs.x - lhs.x), y(rhs.y - lhs.y) {};

long long Vector::CrossProduct(const Vector& lhs, const Vector& rhs) {
    return lhs.x * rhs.y - rhs.x * lhs.y;
}

Vector& Vector::operator=(const Vector& rhs) {
    x = rhs.x;
    y = rhs.y;
    return(*this);
}

Vector& operator+=(Vector& lhs, const Vector& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

Vector operator+(const Vector& lhs, const Vector& rhs) {
    Vector result = lhs;
    result += rhs;
    return result;
}

Vector& operator-=(Vector& lhs, const Vector& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

Vector operator-(const Vector& lhs, const Vector& rhs) {
    Vector result = lhs;
    result -= rhs;
    return result;
}

Turn Orientation(const Point& first, const Point& second, const Point& third) {
    Vector lhs(first, second);
    Vector rhs(first, third);
    double turn = Vector::CrossProduct(lhs, rhs);
    if(turn > EPSILON) {
        return Turn::LEFT;
    }
    if(turn < -EPSILON) {
        return Turn::RIGHT;
    }
    return Turn::COLLINEAR;
}

long long OrientedArea (const Point& first, const Point& second, const Point& third) {
    Vector lhs(first, second);
    Vector rhs(first, third);
    return Vector::CrossProduct(lhs, rhs);
}

bool AreProjectionsIntersect(long long firstStart, long long  firstEnd, long long secondStart, long long  secondEnd) {
    if(firstStart > firstEnd) {
        std::swap(firstStart, firstEnd);
    }
    if(secondStart > secondEnd)  {
        std::swap(secondStart, secondEnd);
    }
    return std::max(firstStart, secondStart) <= std::min(firstEnd, secondEnd);
}
 
class Segment {
public:
    Segment(const Point& start, const Point& end);
    
    long double GetYForCmp(long long x) const;
    
    Point start;
    Point end;
    
};

Segment::Segment(const Point& start, const Point& end) : start(start), end(end) {};

bool AreSegmentsCross(const Segment& lhs, const Segment& rhs) {
    return AreProjectionsIntersect(lhs.start.x, lhs.end.x, rhs.start.x, rhs.end.x)
        && AreProjectionsIntersect(lhs.start.y, lhs.end.y, rhs.start.y, rhs.end.y)
        && OrientedArea(lhs.start, lhs.end, rhs.start) * OrientedArea(lhs.start, lhs.end, rhs.end) <= 0
        && OrientedArea(rhs.start, rhs.end, lhs.start) * OrientedArea(rhs.start, rhs.end, lhs.end) <= 0;
}

int main() {
    Point A;
    Point B;
    std::cin >> A >> B;
    Segment segToCross(A, B);
    int N;
    std::cin >> N;
    int cnt = 0;
    for (int i = 0; i < N; ++i) {
        Point start, end;
        std::cin >> start >> end;
        Segment seg(start, end);
        if (AreSegmentsCross(segToCross, seg)) {
            ++cnt;
        }
    }
    std::cout << cnt;
    return 0;
}
