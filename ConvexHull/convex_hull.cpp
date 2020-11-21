#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <cmath>

const double EPSILON = 0.0001;

enum Turn {
    LEFT,
    RIGHT,
    COLLINEAR
};

bool areEqual(double first, double second) {
    return abs(first - second) <= EPSILON;
}

class Point {
public:
    Point();
    Point(long long x, long long y);
    Point(const Point&);
    
    Point& operator=(const Point&);
    
    friend bool operator ==(const Point&, const Point&);
    friend bool operator !=(const Point&, const Point&);
    
    friend bool operator <(const Point&, const Point&);
    friend bool operator >(const Point&, const Point&);
    friend bool operator <=(const Point&, const Point&);
    friend bool operator >=(const Point&, const Point&);
    
    friend Point& operator+=(Point&, const Point&);
    friend Point operator+(const Point&, const Point&);
    
    friend Point& operator-=(Point&, const Point&);
    friend Point operator-(const Point&, const Point&);

    friend void PrintPoint(const Point&);
    
    long long x;
    long long y;
};

Point::Point() : x(0), y(0) {};
Point::Point(long long x, long long y) : x(x), y(y) {};
Point::Point(const Point& rhs) : x(rhs.x), y(rhs.y) {};

Point& Point::operator=(const Point& rhs) {
    x = rhs.x;
    y = rhs.y;
    return (*this);
}

bool operator ==(const Point& lhs, const Point& rhs) {
    return (lhs.x == rhs.x) && (lhs.y == rhs.y);
}

bool operator !=(const Point& lhs, const Point& rhs) {
    return !(lhs == rhs);
}

bool operator <(const Point& lhs, const Point& rhs) {
    return (lhs.x < rhs.x) || (lhs.x == rhs.x && lhs.y < rhs.y);
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

Point& operator+=(Point& lhs, const Point& rhs) {
    lhs.x += rhs.x;
    lhs.y += rhs.y;
    return lhs;
}

Point operator+(const Point& lhs, const Point& rhs) {
    Point result = lhs;
    result += rhs;
    return result;
}

Point& operator-=(Point& lhs, const Point& rhs) {
    lhs.x -= rhs.x;
    lhs.y -= rhs.y;
    return lhs;
}

Point operator-(const Point& lhs, const Point& rhs) {
    Point result = lhs;
    result -= rhs;
    return result;
}

long double DistanceBetweenPoints(const Point& lhs, const Point& rhs) {
    return sqrt((rhs.x - lhs.x) * (rhs.x - lhs.x) +
                (rhs.y - lhs.y) * (rhs.y - lhs.y));
}

void PrintPoint(const Point& point) {
    std::cout << point.x << " " << point.y << std::endl;
}

double CrossProduct(const Point& first, const Point& second) {
    return first.x * second.y - second.x * first.y;
}

Turn Orientation(const Point& first, const Point& second, const Point& third) {
    double turn = CrossProduct(second - first, third - first);
    if (turn > EPSILON) {
        return LEFT;
    }
    if (turn < -EPSILON) {
        return RIGHT;
    }
    return COLLINEAR;
}

std::vector<Point> BuildConvexHull(std::vector<Point> points) {
    std::vector<Point> convexHull;
    
    if (points.size() == 1) {
        convexHull.emplace_back(points.front());
        return convexHull;
    }
    
    std::sort(points.begin(), points.end());
    
    std::vector<Point> upHull;
    std::vector<Point> bottomHull;
    
    Point front = points.front(), back = points.back();
    
    upHull.emplace_back(front);
    bottomHull.emplace_back(front);
    
    auto size = points.size();
    for (auto i = 1; i < size; ++i) {
        if (i == size - 1 || Orientation(front, points[i], back) == RIGHT) {
            while (upHull.size() >= 2 && Orientation(upHull[upHull.size() - 2], upHull[upHull.size() - 1], points[i]) != RIGHT)
                upHull.pop_back();
            upHull.push_back (points[i]);
        }
        if (i == size - 1 || Orientation(front, points[i], back) == LEFT ) {
            while (bottomHull.size() >= 2 && Orientation(bottomHull[bottomHull.size() - 2], bottomHull[bottomHull.size() - 1], points[i]) != LEFT)
                bottomHull.pop_back();
            bottomHull.push_back (points[i]);
        }
    }
    for (auto i = 0; i < upHull.size(); ++i) {
        convexHull.push_back (upHull[i]);
    }
    for (auto i = bottomHull.size() - 2; i > 0; --i) {
        convexHull.push_back (bottomHull[i]);
    }
    
    return convexHull;
}

long double Perimeter(const std::vector<Point>& polygon) {
    long double perimeter = 0.;
    int size = polygon.size();
    for (auto i = 0; i < size; ++i) {
        perimeter += DistanceBetweenPoints(polygon[i], polygon[(i + 1) % size]);
    }
    
    return perimeter;
}
int main() {
    int N;
    std::cin >> N;
    std::vector<Point> points;
    for (int i = 0; i < N; ++i) {
        Point point;
        std::cin >> point.x >> point.y;
        points.emplace_back(point);
    }
    std::vector<Point> convexHull = BuildConvexHull(points);
    std::cout << std::setprecision(10) << Perimeter(convexHull) << std::fixed << std::endl;
    return 0;
}
