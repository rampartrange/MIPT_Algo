#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>

const long double EPSILON = 0.000001;

enum Turn {
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

long double Angle(const Point& lhs, const Point& rhs) {
    Point vector = rhs - lhs;
    long double tg = atan2(vector.y, vector.x);
    if (tg <= -M_PI_2) {
        tg += 2 * M_PI;
    }
//    if (vector.x < 0 && vector.y < 0) {
//        tg += 2 * M_PI;
//    }
    //std::cout << "X : " << vector.x << " Y : " << vector.y << " atan : " << tg << std::endl;
     return tg;
}

std::vector<Point> ProcessPolygon(const std::vector<Point>& polygon) {
    std::vector<Point> processedPolygon;
    
    auto indexMinPoint = 0;
    auto size = polygon.size();
    
    for (auto i = 0; i < size; ++i) {
        if (polygon[i] < polygon[indexMinPoint]) {
            indexMinPoint = i;
        }
    }
    
    for (auto i = 0; i < size; ++i) {
        processedPolygon.emplace_back(polygon[(indexMinPoint + i) % size]);
    }
    
    return processedPolygon;
}

std::vector<Point> MinkowskiAddition(const std::vector<Point>& firstPolygon, const std::vector<Point>& secondPolygon) {
    std::vector<Point> result;
    
    std::vector<Point> processedFirstPolygon = ProcessPolygon(firstPolygon);
    std::vector<Point> processedSecondPolygon = ProcessPolygon(secondPolygon);
    
    processedFirstPolygon.emplace_back(processedFirstPolygon[0]);
    processedFirstPolygon.emplace_back(processedFirstPolygon[1]);
    
    processedSecondPolygon.emplace_back(processedSecondPolygon[0]);
    processedSecondPolygon.emplace_back(processedSecondPolygon[1]);
    
    auto firstSize = firstPolygon.size();
    auto secondSize = secondPolygon.size();
    
    auto i = 0, j = 0;
    
    while (i < firstSize || j < secondSize) {
        result.emplace_back(processedFirstPolygon[i] + processedSecondPolygon[j]);
        
        if (i >= firstSize) {
            ++j;
            continue;
        }
        
        if (j >= secondSize) {
            ++i;
            continue;
        }
        
        if (Angle(processedFirstPolygon[i], processedFirstPolygon[i + 1]) < Angle(processedSecondPolygon[j], processedSecondPolygon[j + 1])) {
            ++i;
        } else if (Angle(processedFirstPolygon[i], processedFirstPolygon[i + 1]) > Angle(processedSecondPolygon[j], processedSecondPolygon[j + 1])) {
            ++j;
        } else {
            ++i;
            ++j;
        }
        //std::cout << std::endl;
    }
    
    return result;
}

long double PolygonArea(std::vector<Point>& polygon) {
    long long area = 0.;
    auto size = polygon.size();
    for (auto i = 0; i < size; ++i) {
        area += polygon[i].x * polygon[(i + 1) % size].y;
        area -= polygon[i].y * polygon[(i + 1) % size].x;
    }
    return static_cast<long double>(area) / 2.0;
}

long double MixedArea(std::vector<Point>& lhs, std::vector<Point>& rhs) {
    std::vector<Point> minkowskiAddition = MinkowskiAddition(lhs, rhs);
//    for (const auto& point : minkowskiAddition) {
//        std::cout << point.x << " " << point.y << " " << std::endl;
//    }
    long double result =  PolygonArea(minkowskiAddition) - PolygonArea(lhs) - PolygonArea(rhs);
    return result / 2.0;
}

int main() {
    int N;
    std::cin >> N;
    std::vector<Point> firstPolygon;
    for (int i = 0; i < N; ++i) {
        long long x, y;
        std::cin >> x >> y;
        firstPolygon.emplace_back(Point(x, y));
    }
    
    int M;
    std::cin >> M;
    std::vector<Point> secondPolygon;
    for (int i = 0; i < M; ++i) {
        long long x, y;
        std::cin >> x >> y;
        secondPolygon.emplace_back(Point(x, y));
    }
    std::cout << std::setprecision(6) << std::fixed;
    std::cout << MixedArea(firstPolygon, secondPolygon) << std::endl;
    return 0;
}
