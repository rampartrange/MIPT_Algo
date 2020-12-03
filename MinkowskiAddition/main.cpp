#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <tuple>

const long double EPSILON = 0.000001;
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

long double Angle(const Point& lhs, const Point& rhs) {
    Vector vector(lhs, rhs);
    long double tg = atan2(vector.y, vector.x);
    if (tg <= -M_PI_2) {
        tg += 2 * M_PI;
    }
    
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
        Point additionPoint(processedFirstPolygon[i].x + processedSecondPolygon[j].x,
                            processedFirstPolygon[i].y + processedSecondPolygon[j].y);
        result.emplace_back(additionPoint);
        
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
    std::cout << std::setprecision(PRECISION) << std::fixed;
    std::cout << MixedArea(firstPolygon, secondPolygon) << std::endl;
    return 0;
}
