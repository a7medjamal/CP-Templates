#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double PI = acos(-1.0);

// Advanced geometric algorithms
namespace AdvancedGeometry {
    
    struct Point {
        double x, y;
        int id;
        
        Point() : x(0), y(0), id(-1) {}
        Point(double x, double y, int id = -1) : x(x), y(y), id(id) {}
        
        Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
        Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
        Point operator*(double t) const { return Point(x * t, y * t); }
        Point operator/(double t) const { return Point(x / t, y / t); }
        
        double dot(const Point& p) const { return x * p.x + y * p.y; }
        double cross(const Point& p) const { return x * p.y - y * p.x; }
        double norm() const { return sqrt(x * x + y * y); }
        double norm2() const { return x * x + y * y; }
        
        bool operator<(const Point& p) const {
            return x < p.x || (abs(x - p.x) < EPS && y < p.y);
        }
        
        bool operator==(const Point& p) const {
            return abs(x - p.x) < EPS && abs(y - p.y) < EPS;
        }
        
        double distanceTo(const Point& p) const {
            return (*this - p).norm();
        }
        
        double distanceToSquared(const Point& p) const {
            return (*this - p).norm2();
        }
    };
    
    typedef vector<Point> Polygon;
    
    // Cross product of vectors OA and OB
    double cross(const Point& O, const Point& A, const Point& B) {
        return (A - O).cross(B - O);
    }
    
    // Closest pair of points using divide and conquer
    class ClosestPair {
    public:
        static pair<Point, Point> findClosestPair(vector<Point> points) {
            if (points.size() < 2) return {Point(), Point()};
            
            sort(points.begin(), points.end());
            return closestPairRec(points, 0, points.size() - 1);
        }
        
    private:
        static pair<Point, Point> closestPairRec(const vector<Point>& points, int left, int right) {
            int n = right - left + 1;
            
            // Base case: brute force for small arrays
            if (n <= 3) {
                return bruteForce(points, left, right);
            }
            
            int mid = left + (right - left) / 2;
            Point midPoint = points[mid];
            
            auto leftPair = closestPairRec(points, left, mid);
            auto rightPair = closestPairRec(points, mid + 1, right);
            
            double leftDist = leftPair.first.distanceTo(leftPair.second);
            double rightDist = rightPair.first.distanceTo(rightPair.second);
            
            double minDist = min(leftDist, rightDist);
            auto minPair = (leftDist < rightDist) ? leftPair : rightPair;
            
            // Check points near the dividing line
            vector<Point> strip;
            for (int i = left; i <= right; i++) {
                if (abs(points[i].x - midPoint.x) < minDist) {
                    strip.push_back(points[i]);
                }
            }
            
            // Sort strip by y-coordinate
            sort(strip.begin(), strip.end(), [](const Point& a, const Point& b) {
                return a.y < b.y;
            });
            
            // Check distances in strip
            for (int i = 0; i < (int)strip.size(); i++) {
                for (int j = i + 1; j < (int)strip.size() && (strip[j].y - strip[i].y) < minDist; j++) {
                    double dist = strip[i].distanceTo(strip[j]);
                    if (dist < minDist) {
                        minDist = dist;
                        minPair = {strip[i], strip[j]};
                    }
                }
            }
            
            return minPair;
        }
        
        static pair<Point, Point> bruteForce(const vector<Point>& points, int left, int right) {
            double minDist = 1e18;
            pair<Point, Point> result;
            
            for (int i = left; i <= right; i++) {
                for (int j = i + 1; j <= right; j++) {
                    double dist = points[i].distanceTo(points[j]);
                    if (dist < minDist) {
                        minDist = dist;
                        result = {points[i], points[j]};
                    }
                }
            }
            
            return result;
        }
    };
    
    // Line sweep algorithm for rectangle union area
    class LineSweep {
    public:
        struct Rectangle {
            double x1, y1, x2, y2;
            Rectangle(double x1, double y1, double x2, double y2) 
                : x1(min(x1, x2)), y1(min(y1, y2)), x2(max(x1, x2)), y2(max(y1, y2)) {}
        };
        
        struct Event {
            double x;
            double y1, y2;
            int type; // 1 for start, -1 for end
            
            bool operator<(const Event& other) const {
                if (abs(x - other.x) > EPS) return x < other.x;
                return type > other.type; // Process start events before end events
            }
        };
        
        static double rectangleUnionArea(const vector<Rectangle>& rectangles) {
            vector<Event> events;
            set<double> yCoords;
            
            for (const auto& rect : rectangles) {
                events.push_back({rect.x1, rect.y1, rect.y2, 1});
                events.push_back({rect.x2, rect.y1, rect.y2, -1});
                yCoords.insert(rect.y1);
                yCoords.insert(rect.y2);
            }
            
            sort(events.begin(), events.end());
            
            vector<double> y(yCoords.begin(), yCoords.end());
            vector<int> count(y.size() - 1, 0);
            
            double totalArea = 0;
            double lastX = events[0].x;
            
            for (const auto& event : events) {
                // Calculate area contribution
                double currentHeight = 0;
                for (int i = 0; i < (int)count.size(); i++) {
                    if (count[i] > 0) {
                        currentHeight += y[i + 1] - y[i];
                    }
                }
                totalArea += currentHeight * (event.x - lastX);
                
                // Update count array
                int y1Idx = lower_bound(y.begin(), y.end(), event.y1) - y.begin();
                int y2Idx = lower_bound(y.begin(), y.end(), event.y2) - y.begin();
                
                for (int i = y1Idx; i < y2Idx; i++) {
                    count[i] += event.type;
                }
                
                lastX = event.x;
            }
            
            return totalArea;
        }
    };
    
    // Rotating calipers for convex polygon
    class RotatingCalipers {
    public:
        // Find maximum distance between any two points in convex polygon
        static pair<Point, Point> maxDistance(const Polygon& convexPoly) {
            int n = convexPoly.size();
            if (n < 2) return {Point(), Point()};
            if (n == 2) return {convexPoly[0], convexPoly[1]};
            
            double maxDist = 0;
            pair<Point, Point> result;
            
            int j = 1;
            for (int i = 0; i < n; i++) {
                while (true) {
                    int next_j = (j + 1) % n;
                    Point edge = convexPoly[(i + 1) % n] - convexPoly[i];
                    Point vec1 = convexPoly[j] - convexPoly[i];
                    Point vec2 = convexPoly[next_j] - convexPoly[i];
                    
                    if (abs(edge.cross(vec2)) <= abs(edge.cross(vec1))) break;
                    j = next_j;
                }
                
                double dist = convexPoly[i].distanceTo(convexPoly[j]);
                if (dist > maxDist) {
                    maxDist = dist;
                    result = {convexPoly[i], convexPoly[j]};
                }
            }
            
            return result;
        }
        
        // Find minimum width of convex polygon
        static double minWidth(const Polygon& convexPoly) {
            int n = convexPoly.size();
            if (n < 3) return 0;
            
            double minWidth = 1e18;
            
            for (int i = 0; i < n; i++) {
                Point edge = convexPoly[(i + 1) % n] - convexPoly[i];
                double edgeLength = edge.norm();
                
                if (edgeLength < EPS) continue;
                
                double maxDist = 0;
                for (int j = 0; j < n; j++) {
                    Point vec = convexPoly[j] - convexPoly[i];
                    double dist = abs(vec.cross(edge)) / edgeLength;
                    maxDist = max(maxDist, dist);
                }
                
                minWidth = min(minWidth, maxDist);
            }
            
            return minWidth;
        }
        
        // Find minimum area rectangle enclosing convex polygon
        static double minEnclosingRectangleArea(const Polygon& convexPoly) {
            int n = convexPoly.size();
            if (n < 3) return 0;
            
            double minArea = 1e18;
            
            for (int i = 0; i < n; i++) {
                Point edge = convexPoly[(i + 1) % n] - convexPoly[i];
                double edgeLength = edge.norm();
                
                if (edgeLength < EPS) continue;
                
                Point edgeUnit = edge / edgeLength;
                Point perpUnit(-edgeUnit.y, edgeUnit.x);
                
                double minProj = 1e18, maxProj = -1e18;
                double minPerpProj = 1e18, maxPerpProj = -1e18;
                
                for (const Point& p : convexPoly) {
                    Point vec = p - convexPoly[i];
                    double proj = vec.dot(edgeUnit);
                    double perpProj = vec.dot(perpUnit);
                    
                    minProj = min(minProj, proj);
                    maxProj = max(maxProj, proj);
                    minPerpProj = min(minPerpProj, perpProj);
                    maxPerpProj = max(maxPerpProj, perpProj);
                }
                
                double width = maxProj - minProj;
                double height = maxPerpProj - minPerpProj;
                minArea = min(minArea, width * height);
            }
            
            return minArea;
        }
    };
    
    // Simplified Voronoi diagram using Fortune's algorithm
    class VoronoiDiagram {
    public:
        struct Site {
            Point point;
            int id;
            Site(Point p, int id) : point(p), id(id) {}
        };
        
        struct Edge {
            Point start, end;
            int leftSite, rightSite;
            Edge(Point s, Point e, int l, int r) : start(s), end(e), leftSite(l), rightSite(r) {}
        };
        
        static vector<Edge> computeVoronoi(const vector<Point>& points, double boundingBox = 1000) {
            // Simplified implementation - returns approximate Voronoi edges
            vector<Edge> edges;
            int n = points.size();
            
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    // Perpendicular bisector of points i and j
                    Point mid = (points[i] + points[j]) * 0.5;
                    Point dir = points[j] - points[i];
                    Point perp(-dir.y, dir.x);
                    perp = perp / perp.norm();
                    
                    // Extend in both directions
                    Point start = mid - perp * boundingBox;
                    Point end = mid + perp * boundingBox;
                    
                    edges.push_back(Edge(start, end, i, j));
                }
            }
            
            return edges;
        }
        
        // Check if point is closer to site than to any other site
        static bool isInVoronoiCell(const Point& p, int siteIndex, const vector<Point>& sites) {
            double distToSite = p.distanceTo(sites[siteIndex]);
            
            for (int i = 0; i < (int)sites.size(); i++) {
                if (i != siteIndex && p.distanceTo(sites[i]) < distToSite - EPS) {
                    return false;
                }
            }
            
            return true;
        }
    };
    
    // Simplified Delaunay triangulation
    class DelaunayTriangulation {
    public:
        struct Triangle {
            int a, b, c; // Indices of vertices
            Triangle(int a, int b, int c) : a(a), b(b), c(c) {}
        };
        
        // Check if triangle satisfies Delaunay condition
        static bool isDelaunay(const Triangle& tri, const vector<Point>& points, const Point& testPoint) {
            const Point& a = points[tri.a];
            const Point& b = points[tri.b];
            const Point& c = points[tri.c];
            
            // Check if testPoint is inside circumcircle of triangle abc
            double d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
            if (abs(d) < EPS) return true; // Degenerate triangle
            
            double ux = ((a.x * a.x + a.y * a.y) * (b.y - c.y) + 
                         (b.x * b.x + b.y * b.y) * (c.y - a.y) + 
                         (c.x * c.x + c.y * c.y) * (a.y - b.y)) / d;
            double uy = ((a.x * a.x + a.y * a.y) * (c.x - b.x) + 
                         (b.x * b.x + b.y * b.y) * (a.x - c.x) + 
                         (c.x * c.x + c.y * c.y) * (b.x - a.x)) / d;
            
            Point circumcenter(ux, uy);
            double circumradius = circumcenter.distanceTo(a);
            
            return circumcenter.distanceTo(testPoint) > circumradius + EPS;
        }
        
        // Simple Delaunay triangulation (O(n^4) - for small point sets)
        static vector<Triangle> computeDelaunay(const vector<Point>& points) {
            vector<Triangle> triangles;
            int n = points.size();
            
            for (int i = 0; i < n; i++) {
                for (int j = i + 1; j < n; j++) {
                    for (int k = j + 1; k < n; k++) {
                        // Check if triangle ijk is Delaunay
                        bool valid = true;
                        
                        for (int l = 0; l < n && valid; l++) {
                            if (l != i && l != j && l != k) {
                                Triangle tri(i, j, k);
                                if (!isDelaunay(tri, points, points[l])) {
                                    valid = false;
                                }
                            }
                        }
                        
                        if (valid) {
                            triangles.push_back(Triangle(i, j, k));
                        }
                    }
                }
            }
            
            return triangles;
        }
    };
    
    // Convex hull using Graham scan
    Polygon convexHull(vector<Point> points) {
        int n = points.size();
        if (n <= 1) return points;
        
        // Find the bottom-most point (and leftmost in case of tie)
        int minIdx = 0;
        for (int i = 1; i < n; i++) {
            if (points[i].y < points[minIdx].y || 
                (abs(points[i].y - points[minIdx].y) < EPS && points[i].x < points[minIdx].x)) {
                minIdx = i;
            }
        }
        swap(points[0], points[minIdx]);
        
        Point pivot = points[0];
        
        // Sort points by polar angle with respect to pivot
        sort(points.begin() + 1, points.end(), [&](const Point& a, const Point& b) {
            double crossProd = cross(pivot, a, b);
            if (abs(crossProd) < EPS) {
                return pivot.distanceTo(a) < pivot.distanceTo(b);
            }
            return crossProd > 0;
        });
        
        // Build convex hull
        vector<Point> hull;
        for (const Point& p : points) {
            while (hull.size() >= 2 && cross(hull[hull.size()-2], hull[hull.size()-1], p) <= EPS) {
                hull.pop_back();
            }
            hull.push_back(p);
        }
        
        return hull;
    }
    
    // Half-plane intersection
    struct HalfPlane {
        Point a, b; // Line from a to b, half-plane is to the left
        HalfPlane(Point a, Point b) : a(a), b(b) {}
        
        Point direction() const { return b - a; }
        
        bool contains(const Point& p) const {
            return cross(a, b, p) >= -EPS;
        }
        
        Point intersect(const HalfPlane& other) const {
            Point d1 = direction();
            Point d2 = other.direction();
            double det = d1.cross(d2);
            
            if (abs(det) < EPS) return Point(1e18, 1e18); // Parallel
            
            Point diff = other.a - a;
            double t = diff.cross(d2) / det;
            return a + d1 * t;
        }
    };
    
    Polygon halfPlaneIntersection(vector<HalfPlane> planes) {
        // Sort half-planes by angle
        sort(planes.begin(), planes.end(), [](const HalfPlane& a, const HalfPlane& b) {
            Point da = a.direction();
            Point db = b.direction();
            return atan2(da.y, da.x) < atan2(db.y, db.x);
        });
        
        deque<HalfPlane> dq;
        deque<Point> pts;
        
        for (const HalfPlane& plane : planes) {
            // Remove half-planes that are no longer relevant
            while (dq.size() >= 2 && !plane.contains(pts.back())) {
                dq.pop_back();
                pts.pop_back();
            }
            
            while (dq.size() >= 2 && !plane.contains(pts.front())) {
                dq.pop_front();
                pts.pop_front();
            }
            
            if (!dq.empty()) {
                Point intersection = dq.back().intersect(plane);
                if (intersection.x < 1e17) { // Valid intersection
                    pts.push_back(intersection);
                }
            }
            
            dq.push_back(plane);
        }
        
        // Final cleanup
        while (dq.size() >= 2 && !dq.front().contains(pts.back())) {
            dq.pop_back();
            pts.pop_back();
        }
        
        if (dq.size() >= 2) {
            Point intersection = dq.back().intersect(dq.front());
            if (intersection.x < 1e17) {
                pts.push_back(intersection);
            }
        }
        
        return vector<Point>(pts.begin(), pts.end());
    }
}

// Example usage:
/*
int main() {
    using namespace AdvancedGeometry;
    
    // Closest pair
    vector<Point> points = {Point(0, 0), Point(1, 1), Point(2, 2), Point(10, 10)};
    auto closestPair = ClosestPair::findClosestPair(points);
    cout << "Closest pair distance: " << closestPair.first.distanceTo(closestPair.second) << endl;
    
    // Rectangle union area
    vector<LineSweep::Rectangle> rectangles = {
        LineSweep::Rectangle(0, 0, 2, 2),
        LineSweep::Rectangle(1, 1, 3, 3),
        LineSweep::Rectangle(2, 0, 4, 1)
    };
    double unionArea = LineSweep::rectangleUnionArea(rectangles);
    cout << "Rectangle union area: " << unionArea << endl;
    
    // Convex hull
    vector<Point> hullPoints = {Point(0, 0), Point(1, 1), Point(2, 0), Point(1, 2), Point(0, 2)};
    Polygon hull = convexHull(hullPoints);
    cout << "Convex hull has " << hull.size() << " vertices" << endl;
    
    // Rotating calipers
    if (hull.size() >= 3) {
        auto diameter = RotatingCalipers::maxDistance(hull);
        cout << "Maximum distance in convex hull: " << diameter.first.distanceTo(diameter.second) << endl;
        
        double minWidth = RotatingCalipers::minWidth(hull);
        cout << "Minimum width of convex hull: " << minWidth << endl;
    }
    
    // Delaunay triangulation
    vector<Point> triPoints = {Point(0, 0), Point(1, 0), Point(0, 1), Point(1, 1)};
    auto triangles = DelaunayTriangulation::computeDelaunay(triPoints);
    cout << "Delaunay triangulation has " << triangles.size() << " triangles" << endl;
    
    return 0;
}
*/