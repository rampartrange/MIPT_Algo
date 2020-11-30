#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <vector>
#include <set>

#define double long double

const double EPSILON = 1e-7;
const double INF = std::numeric_limits<double>::max();
const int PRECISION = 8;

bool AreEqual(double first, double second) {
    return abs(first - second) <= EPSILON;
}

class Point3D {
public:
    explicit Point3D(double x = 0, double y = 0, double z = 0, int id = -1, Point3D* next = NULL, Point3D* prev = NULL);
    
    Point3D& operator=(const Point3D&);
    
    Point3D* next;
    Point3D* prev;
    
    enum class OperationType{
        INSERT,
        DELETE
    };
    
    OperationType ProcessEvent();
    
    void RotatePoint(double);
    
    double x;
    double y;
    double z;
    int id;
    
private:
    void RotateCoordinates2D(double& first, double& second, double phi);
};

Point3D::Point3D(double x, double y, double z, int id, Point3D* prev, Point3D* next) : x(x), y(y), z(z), id(id), prev(prev), next(next) {};

Point3D& Point3D::operator=(const Point3D& rhs) {
    x = rhs.x;
    y = rhs.y;
    z = rhs.z;
    id = rhs.id;
    return (*this);
}

bool operator ==(const Point3D& lhs, const Point3D& rhs) {
    return std::tie(lhs.x, lhs.y, lhs.z) == std::tie(rhs.x, rhs.y, rhs.z);
}

bool operator !=(const Point3D& lhs, const Point3D& rhs) {
    return !(lhs == rhs);
}

bool operator <(const Point3D& lhs, const Point3D& rhs) {
    return std::tie(lhs.x, lhs.y, lhs.z) < std::tie(rhs.x, rhs.y, rhs.z);
}

bool operator >(const Point3D& lhs, const Point3D& rhs) {
    return rhs < lhs;
}

bool operator <=(const Point3D& lhs, const Point3D& rhs) {
    return !(lhs > rhs);
}

bool operator >=(const Point3D& lhs, const Point3D& rhs) {
    return !(lhs < rhs);
}

std::istream& operator>>(std::istream& is, Point3D& point) {
    is >> point.x >> point.y >> point.z;
    return is;
}

std::ostream& operator<<(std::ostream& os, Point3D& point) {
    os << point.x << " " << point.y << " " << point.z;
    return os;
}

Point3D::OperationType Point3D::ProcessEvent() {
    if (prev->next == this) {
        prev->next = next;
        next->prev = prev;
        return OperationType::DELETE;
    } else {
        prev->next = this;
        next->prev = this;
        return OperationType::INSERT;
    }
}

void Point3D::RotateCoordinates2D(double& first, double& second, double phi) {
    double _first = 0;
    double _second = 0;
    
    _first = first * cos(phi) - second * sin(phi);
    _second = first * sin(phi) + second * cos(phi);
    
    first = _first;
    second = _second;
}

void Point3D::RotatePoint(double phi) {
    RotateCoordinates2D(x, y, phi);
    RotateCoordinates2D(x, z, phi);
    RotateCoordinates2D(y, z, phi);
}

double CrossProductOXY(const Point3D* first, const Point3D* second, const Point3D* third) {
    if (first == NULL || second == NULL || third == NULL) {
        return INF;
    }
    return (second->x - first->x) * (third->y - second->y) - (second->y - first->y) * (third->x - second->x);
}

double CrossProductOXZ(const Point3D* first, const Point3D* second, const Point3D* third) {
    if (first == NULL || second == NULL || third == NULL) {
        return INF;
    }
    return (second->x - first->x) * (third->z - second->z) - (second->z - first->z) * (third->x - second->x);
}

enum class Turn : int {
    LEFT = 1,
    RIGHT = -1,
    COLLINEAR = 0
};

Turn OrientationOXY(const Point3D* first, const Point3D* second, const Point3D* third) {
    double turn = CrossProductOXY(first, second, third);
    
    if(turn > EPSILON) {
        return Turn::LEFT;
    }
    
    if(turn < -EPSILON) {
        return Turn::RIGHT;
    }
    
    return Turn::COLLINEAR;
}


double TimeChange(const Point3D* first, const Point3D* second, const Point3D* third) {
    if (first == NULL || second == NULL || third == NULL) {
        return INF;
    }
   
    return CrossProductOXZ(first, second, third) / CrossProductOXY(first, second, third);
}


using Movie = std::vector<Point3D*>;

int GetIndex(Point3D* point) {
    return point->id;
}

void ComputeBridgePoints(Point3D*& u, Point3D*& v) {
    while (true) {
        if (OrientationOXY(u, v, v->next) == Turn::RIGHT) {
            v = v->next;
        } else if (OrientationOXY(u->prev, u, v) == Turn::RIGHT) {
            u = u->prev;
        } else {
            break;
        }
    }
}

void ProcessEvent(Point3D*& u, Point3D*& v, int type, Movie& result, Point3D*& rightHullPtr, int& rightHullIndex, Point3D*& leftHullPtr, int& leftHullIndex) {
    if (type == 0) {
        if (leftHullPtr->x < u->x) {
            result.emplace_back(leftHullPtr);
        }
        leftHullPtr->ProcessEvent();
        ++leftHullIndex;
    } else if (type == 1) {
        if (rightHullPtr->x > v->x) {
            result.emplace_back(rightHullPtr);
        }
        rightHullPtr->ProcessEvent();
        ++rightHullIndex;
    } else if (type == 2) {
        result.emplace_back(u);
        u = u->prev;
    } else if (type == 3) {
        u = u->next;
        result.emplace_back(u);
    } else if (type == 4) {
        v = v->prev;
        result.emplace_back(v);
    } else if (type == 5) {
        result.emplace_back(v);
        v = v->next;
    }
}

Movie MergeHulls(Movie& leftHull, Movie& rightHull, Point3D*& u, Point3D*& v) {
    Movie result;

    int leftHullIndex = 0;
    int rightHullIndex = 0;
    
    auto leftHullSize = leftHull.size();
    auto rightHullSize = rightHull.size();
    
    double currentTime = -INF;
    
    while (true) {
        std::vector<double> t(6, INF);
        
        Point3D* leftHullPtr = NULL;
        Point3D* rightHullPtr = NULL;
        
        if (leftHullIndex < leftHullSize) {
            leftHullPtr = leftHull[leftHullIndex];
            t[0] = TimeChange(leftHullPtr->prev, leftHullPtr, leftHullPtr->next);
        }
        
        if (rightHullIndex < rightHullSize) {
            rightHullPtr = rightHull[rightHullIndex];
            t[1] = TimeChange(rightHullPtr->prev, rightHullPtr, rightHullPtr->next);
        }
        
        t[2] = TimeChange(u->prev, u, v);
        t[3] = TimeChange(u, u->next, v);
        t[4] = TimeChange(u, v->prev, v);
        t[5] = TimeChange(u, v, v->next);
        
        int indexClosestEvent = 0;
        double minTimeEvent = INF;
        
        for (int i = 0; i < 6; ++i) {
            if (t[i] > currentTime && t[i] < minTimeEvent) {
                minTimeEvent = t[i];
                indexClosestEvent = i;
            }
        }
        
        if (minTimeEvent == INF) {
            break;
        }
        
        ProcessEvent(u, v, indexClosestEvent, result, rightHullPtr, rightHullIndex, leftHullPtr, leftHullIndex);
        currentTime = minTimeEvent;
    }
    
    return result;
}

void IterateBackwards(const Movie& result, Point3D*& u, Point3D*& v, std::vector<Point3D>& vertices, int middle) {
    for (ssize_t i = result.size() - 1; i >= 0; --i) {
        Point3D* CurrentEvent = result[i];

        if (CurrentEvent->x <= u->x || CurrentEvent->x >= v->x) {
            CurrentEvent->ProcessEvent();
            if (CurrentEvent == u) {
                u = u->prev;
            } else if (CurrentEvent == v) {
                v = v->next;
            }
        } else {
            u->next = CurrentEvent;
            CurrentEvent->prev = u;
            CurrentEvent->next = v;
            v->prev = CurrentEvent;

            if (CurrentEvent->x <= vertices[middle - 1].x) {
                u = CurrentEvent;
            } else {
                v = CurrentEvent;
            }
        }
    }
}

Movie BuildLowerHull(std::vector<Point3D>& vertices, int left, int right) {
    if (right - left == 1) {
        return Movie();
    }
    
    int middle = (left + right) / 2;
    Movie leftHull = BuildLowerHull(vertices, left, middle);
    Movie rightHull = BuildLowerHull(vertices, middle, right);
    
    Point3D* u = &vertices[middle - 1];
    Point3D* v = &vertices[middle];
    
    ComputeBridgePoints(u, v);
    
    Movie result = MergeHulls(leftHull, rightHull, u, v);
    
    u->next = v;
    v->prev = u;

    IterateBackwards(result, u, v, vertices, middle);

    return result;
}

void InvertZAxis(std::vector<Point3D>& vertices) {
    for (auto& point : vertices) {
        point.z = -point.z;
    }
}

void RemoveReferences(std::vector<Point3D>& vertices) {
    for (auto& point : vertices) {
        point.prev = NULL;
        point.next = NULL;
    }
}

Movie BuildUpperHull(std::vector<Point3D>& vertices, int left, int right) {
    InvertZAxis(vertices);
    RemoveReferences(vertices);
    return BuildLowerHull(vertices, left, right);
}

class Facet {
public:
    Facet();
    Facet(int i, int j, int k);
    Facet(const Facet& rhs);
    
    int i;
    int j;
    int k;
    
    void Normalize();
};

Facet::Facet() : i(0), j(0), k(0) {};
Facet::Facet(int i, int j, int k) : i(i), j(j), k(k) { Normalize(); };
Facet::Facet(const Facet& rhs) : i(rhs.i), j(rhs.j), k(rhs.k) { Normalize(); };

void Facet::Normalize() {
    if (j < k && j < i) {
        std::swap(i, j);
        std::swap(j, k);
    } else if (k < i && k < j) {
        std::swap(i, k);
        std::swap(j, k);
    }
}

bool operator<(const Facet& lhs, const Facet& rhs) {
    return std::tie(lhs.i, lhs.j, lhs.k) < std::tie(rhs.i, rhs.j, rhs.k);
}

std::ostream& operator<<(std::ostream& os, Facet& facet) {
    os << facet.i << " " << facet.j << " " << facet.k;
    return os;
}

void GetFacetsFromHull(const Movie& movie, std::vector<Facet>& facets, Point3D::OperationType type) {
    for(auto* event : movie) {
        Facet newFacet(GetIndex(event->prev), GetIndex(event), GetIndex(event->next));
        
        if (event->ProcessEvent() == type) {
            std::swap(newFacet.i, newFacet.j);
        }
        
        newFacet.Normalize();
        facets.emplace_back(newFacet);
    }
}

std::vector<Facet> BuildConvexHull(std::vector<Point3D> vertices) {
    std::sort(vertices.begin(), vertices.end());
    std::vector<Facet> facets;
    
    auto size = vertices.size();
    
    Movie movie = BuildLowerHull(vertices, 0, size);
    GetFacetsFromHull(movie, facets, Point3D::OperationType::DELETE);
    
    movie = BuildUpperHull(vertices, 0, size);
    GetFacetsFromHull(movie, facets, Point3D::OperationType::INSERT);
    
    std::sort(facets.begin(), facets.end());
    
    return facets;
}

using Edge = std::pair<int, int>;

void AddEdge(std::set<Edge>& edges, int from, int to) {
    Edge edge(from, to);
    if (edge.first > edge.second) {
        std::swap(edge.first, edge.second);
    }
    edges.insert(edge);
}

std::vector<int> FindVoronoiEdgesNum(const std::vector<Point3D>& vertices) {
    std::vector<Facet> convexHull = BuildConvexHull(vertices);
    std::vector<int> edgesNumber;
    std::set<Edge> edges;
    
    for (auto& face : convexHull) {
        AddEdge(edges, face.i, face.j);
        AddEdge(edges, face.j, face.k);
        AddEdge(edges, face.k, face.i);
    }
    
    edgesNumber.resize(vertices.size());
    
    for (const auto& edge : edges) {
        ++edgesNumber[edge.first];
        ++edgesNumber[edge.second];
    }
      
    return edgesNumber;
}

std::vector<Point3D> BuildPlanarHull(const std::vector<Point3D>& vertices) {
    std::vector<Point3D> planarHull;
    
    for (const auto& site : vertices) {
      while (planarHull.size() >= 2) {
          if (OrientationOXY(&planarHull[planarHull.size() - 2],
                             &planarHull.back(),
                             &site) == Turn::RIGHT) {
              break;
        }
          planarHull.pop_back();
      }
        planarHull.push_back(site);
    }
    
    for (ssize_t i = vertices.size() - 2, bottom = planarHull.size(); i >= 0; --i) {
        
      while (planarHull.size() > bottom) {
          if (OrientationOXY(&planarHull[planarHull.size() - 2],
                             &planarHull.back(),
                             &vertices[i]) == Turn::RIGHT) {
              break;
        }
          planarHull.pop_back();
      }
        planarHull.push_back(vertices[i]);
    }
    return planarHull;
}

std::pair<int, int> CalculateEdgesNum(const std::vector<int>& edgesNum, const std::vector<bool>& isPointInPlanarHull) {
    int numberOfPolygons = 0;
    int edgesNumber = 0;
    auto size = edgesNum.size();
    
    for (auto i = 0; i < size; ++i) {
      if (!isPointInPlanarHull[i]) {
        edgesNumber += edgesNum[i];
        ++numberOfPolygons;
      }
    }
    return std::make_pair(numberOfPolygons, edgesNumber);
}

double CalculateAverageVoronoiDiagrammSidesNumber(std::vector<Point3D> vertices) {
    std::vector<int> edgesNum = FindVoronoiEdgesNum(vertices);
    
    std::sort(vertices.begin(), vertices.end());
    
    std::vector<bool> isPointInPlanarHull(vertices.size(), false);
    std::vector<Point3D> planarHull = BuildPlanarHull(vertices);
    
    for (auto& point : planarHull) {
        isPointInPlanarHull[point.id] = true;
    }
    
    auto[numberOfPolygons, edgesNumber] = CalculateEdgesNum(edgesNum, isPointInPlanarHull);
    
    if (!numberOfPolygons) {
      return 0.0;
    }
    
    return static_cast<double>(edgesNumber) / static_cast<double>(numberOfPolygons);
    
    
}

int main() {
    std::vector<Point3D> vertices;
    double x, y;
    int j = 0;
    while (std::cin >> x >> y) {
        double z = x * x + y * y;
        vertices.push_back(Point3D(x, y, z, j));
        vertices[j].RotatePoint(EPSILON);
        ++j;
    }
    
    std::cout << std::setprecision(PRECISION) << std::fixed;
    std::cout << CalculateAverageVoronoiDiagrammSidesNumber(vertices) << std::endl;
 


    return 0;
}
