#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <set>

const long double EPSILON = 0.00001;

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
    
    friend std::ostream &operator<<(std::ostream &output, const Point& point);
    friend std::istream &operator>>(std::istream &input, const Point& point);
    
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
    return(lhs.x < rhs.x) ||(lhs.x == rhs.x && lhs.y < rhs.y);
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

long long CrossProduct(const Point& first, const Point& second) {
    return first.x * second.y - second.x * first.y;
}

Turn Orientation(const Point& first, const Point& second, const Point& third) {
    double turn = CrossProduct(second - first, third - first);
    if(turn > EPSILON) {
        return LEFT;
    }
    if(turn < -EPSILON) {
        return RIGHT;
    }
    return COLLINEAR;
}

long long OrientedArea(const Point& first, const Point& second, const Point& third) {
    
    return CrossProduct(second - first, third - first);
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
    Segment(const Point& start, const Point& end, int num);
    
    long double GetYForCmp(long long x) const;
    
    Point start;
    Point end;
    int numInOrder;
    
};

Segment::Segment(const Point& start, const Point& end, int num) : start(start), end(end), numInOrder(num) {};

bool AreSegmentsCross(const Segment& lhs, const Segment& rhs) {
    return AreProjectionsIntersect(lhs.start.x, lhs.end.x, rhs.start.x, rhs.end.x)
        && AreProjectionsIntersect(lhs.start.y, lhs.end.y, rhs.start.y, rhs.end.y)
        && OrientedArea(lhs.start, lhs.end, rhs.start) * OrientedArea(lhs.start, lhs.end, rhs.end) <= 0
        && OrientedArea(rhs.start, rhs.end, lhs.start) * OrientedArea(rhs.start, rhs.end, lhs.end) <= 0;
}

long double Segment::GetYForCmp(long long x) const{
    Point vec = end - start;
    if (vec.x == 0) {
        return static_cast<long double>(start.y);
    }
    
    return  static_cast<long double>(start.y)  + static_cast<long double>(vec.y) * static_cast<long double>(x - start.x) / static_cast<long double>(vec.x);
}

bool operator<(const Segment& lhs, const Segment& rhs) {
    long long x = std::max (std::min (lhs.start.x, lhs.end.x), std::min (rhs.start.x, rhs.end.x));
    return lhs.GetYForCmp(x) < rhs.GetYForCmp(x);
}

bool operator>(const Segment& lhs, const Segment& rhs) {
    return rhs < lhs;
}

bool operator<=(const Segment& lhs, const Segment& rhs) {
    return !(lhs > rhs);
}

bool operator>=(const Segment& lhs, const Segment& rhs) {
    return !(lhs < rhs);
}

bool operator==(const Segment& lhs, const Segment& rhs) {
    return (lhs <= rhs) && (rhs <= lhs);
}

bool operator!=(const Segment& lhs, const Segment& rhs) {
    return !(lhs == rhs);
}

enum TypeOfPoint {
    START = 1,
    END = -1
};

class Event {
public:
    Event(long long, TypeOfPoint, int);
    
    int index;
    TypeOfPoint type;
    long long coordinate;
};

Event::Event(long long coordinate, TypeOfPoint type, int index) : coordinate(coordinate), type(type), index(index) {};

bool operator<(const Event& lhs, const Event& rhs) {
    return (lhs.coordinate != rhs.coordinate) ? lhs.coordinate < rhs.coordinate : lhs.type > rhs.type;
}

std::set<Segment>::iterator Next(const std::set<Segment>& status, std::set<Segment>::iterator it) {
    return (it == status.end()) ? status.end() : ++it;
}

std::set<Segment>::iterator Prev(const std::set<Segment>& status, std::set<Segment>::iterator it) {
    return (it == status.begin()) ? status.end() : --it;
}

std::pair<long long, long long> FindIntersection(const std::vector<Segment>& segments) {
    auto size = segments.size();
    std::vector<Event> events;
    std::set<Segment> status;
    std::vector<std::set<Segment>::iterator> positions(size);
    
    for (auto i = 0; i < size; ++i) {
        if (segments[i].start < segments[i].end) {
            events.emplace_back(segments[i].start.x, START, i);
            events.emplace_back(segments[i].end.x, END, i);
        } else {
            events.emplace_back(segments[i].end.x, START, i);
            events.emplace_back(segments[i].start.x, END, i);
        }
    }
    
    std::sort(events.begin(), events.end());
    auto eventsSize = events.size();

    for (auto i = 0; i < eventsSize; ++i) {
        int index = events[i].index;
        
        if (events[i].type == START) {
            auto next = status.lower_bound(segments[index]);
            auto prev = Prev(status, next);
            
            if (next != status.end() && AreSegmentsCross((*next), segments[index])) {
                return std::make_pair(next->numInOrder, index);
            }
            
            if (prev != status.end() && AreSegmentsCross((*prev), segments[index]))
                return std::make_pair (prev->numInOrder, index);
            
            positions[index] = status.insert(next, segments[index]);
        }
        else {
            auto next= Next(status, positions[index]);
            auto prev = Prev(status, positions[index]);
            if (next != status.end() && prev != status.end() && AreSegmentsCross((*next), (*prev)))
                return std::make_pair(prev->numInOrder, next->numInOrder);
            status.erase(positions[index]);
        }
    }
    
    return std::make_pair(-1, -1);
}

int main() {
    int N;
    std::cin >> N;
    
    std::vector<Segment> segments;
    
    for (int i = 0; i < N; ++i) {
        Point start, end;
        std::cin >> start >> end;
        segments.emplace_back(Segment(start, end, i));
    }
    
    std::pair<long long, long long> intersection = FindIntersection(segments);
    if (!(intersection.first == -1 && intersection.second == -1)) {
        std::cout << "YES" << std::endl;
        std::cout << intersection.first + 1 << " " << intersection.second + 1 << std::endl;
    } else {
        std::cout << "NO" << std::endl;
    }
    return 0;
}
