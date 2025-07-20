#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double PI = acos(-1.0);

// Polygon operations
namespace PolygonOperations {
    
    struct Point {
        double x, y;
        Point() : x(0), y(0) {}
        Point(double x, double y) : x(x), y(y) {}
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
    };
    
    typedef vector<Point> Polygon;
    
    // Cross product of vectors OA and OB
    double cross(const Point& O, const Point& A, const Point& B) {
        return (A - O).cross(B - O);
    }
    
    // Calculate polygon area (signed area, positive for counter-clockwise)
    double polygonArea(const Polygon& poly) {
        int n = poly.size();
        if (n < 3) return 0;
        
        double area = 0;
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            area += poly[i].cross(poly[j]);
        }
        return area / 2.0;
    }
    
    // Calculate polygon area (always positive)
    double polygonAreaAbs(const Polygon& poly) {
        return abs(polygonArea(poly));
    }
    
    // Calculate polygon perimeter
    double polygonPerimeter(const Polygon& poly) {
        int n = poly.size();
        if (n < 2) return 0;
        
        double perimeter = 0;
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            perimeter += poly[i].distanceTo(poly[j]);
        }
        return perimeter;
    }
    
    // Check if polygon is convex
    bool isConvex(const Polygon& poly) {
        int n = poly.size();
        if (n < 3) return false;
        
        bool hasPos = false, hasNeg = false;
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            int k = (i + 2) % n;
            double crossProd = cross(poly[i], poly[j], poly[k]);
            
            if (crossProd > EPS) hasPos = true;
            if (crossProd < -EPS) hasNeg = true;
            
            if (hasPos && hasNeg) return false;
        }
        return true;
    }
    
    // Point in polygon test (ray casting algorithm)
    bool pointInPolygon(const Point& p, const Polygon& poly) {
        int n = poly.size();
        if (n < 3) return false;
        
        bool inside = false;
        for (int i = 0, j = n - 1; i < n; j = i++) {
            if (((poly[i].y > p.y) != (poly[j].y > p.y)) &&
                (p.x < (poly[j].x - poly[i].x) * (p.y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x)) {
                inside = !inside;
            }
        }
        return inside;
    }
    
    // Point in polygon test for convex polygons (more efficient)
    bool pointInConvexPolygon(const Point& p, const Polygon& poly) {
        int n = poly.size();
        if (n < 3) return false;
        
        bool positive = false, negative = false;
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            double crossProd = cross(poly[i], poly[j], p);
            
            if (crossProd > EPS) positive = true;
            if (crossProd < -EPS) negative = true;
            
            if (positive && negative) return false;
        }
        return true;
    }
    
    // Check if point is on polygon boundary
    bool pointOnPolygonBoundary(const Point& p, const Polygon& poly) {
        int n = poly.size();
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            
            // Check if point is on edge
            if (abs(cross(poly[i], poly[j], p)) < EPS) {
                double minX = min(poly[i].x, poly[j].x);
                double maxX = max(poly[i].x, poly[j].x);
                double minY = min(poly[i].y, poly[j].y);
                double maxY = max(poly[i].y, poly[j].y);
                
                if (p.x >= minX - EPS && p.x <= maxX + EPS &&
                    p.y >= minY - EPS && p.y <= maxY + EPS) {
                    return true;
                }
            }
        }
        return false;
    }
    
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
    
    // Convex hull using Andrew's monotone chain algorithm
    Polygon convexHullAndrew(vector<Point> points) {
        int n = points.size();
        if (n <= 1) return points;
        
        sort(points.begin(), points.end());
        
        // Build lower hull
        vector<Point> lower;
        for (int i = 0; i < n; i++) {
            while (lower.size() >= 2 && cross(lower[lower.size()-2], lower[lower.size()-1], points[i]) <= EPS) {
                lower.pop_back();
            }
            lower.push_back(points[i]);
        }
        
        // Build upper hull
        vector<Point> upper;
        for (int i = n - 1; i >= 0; i--) {
            while (upper.size() >= 2 && cross(upper[upper.size()-2], upper[upper.size()-1], points[i]) <= EPS) {
                upper.pop_back();
            }
            upper.push_back(points[i]);
        }
        
        // Remove last point of each half because it's repeated
        lower.pop_back();
        upper.pop_back();
        
        // Concatenate lower and upper hull
        lower.insert(lower.end(), upper.begin(), upper.end());
        return lower;
    }
    
    // Polygon centroid
    Point polygonCentroid(const Polygon& poly) {
        int n = poly.size();
        if (n == 0) return Point(0, 0);
        if (n == 1) return poly[0];
        if (n == 2) return (poly[0] + poly[1]) / 2.0;
        
        double area = 0;
        Point centroid(0, 0);
        
        for (int i = 0; i < n; i++) {
            int j = (i + 1) % n;
            double crossProd = poly[i].cross(poly[j]);
            area += crossProd;
            centroid = centroid + (poly[i] + poly[j]) * crossProd;
        }
        
        area /= 2.0;
        if (abs(area) < EPS) {
            // Degenerate polygon, return average of vertices
            for (const Point& p : poly) {
                centroid = centroid + p;
            }
            return centroid / n;
        }
        
        return centroid / (6.0 * area);
    }
    
    // Check if two polygons intersect
    bool polygonsIntersect(const Polygon& poly1, const Polygon& poly2) {
        // Check if any vertex of poly1 is inside poly2
        for (const Point& p : poly1) {
            if (pointInPolygon(p, poly2)) return true;
        }
        
        // Check if any vertex of poly2 is inside poly1
        for (const Point& p : poly2) {
            if (pointInPolygon(p, poly1)) return true;
        }
        
        // Check if any edges intersect
        int n1 = poly1.size(), n2 = poly2.size();
        for (int i = 0; i < n1; i++) {
            for (int j = 0; j < n2; j++) {
                Point a1 = poly1[i], b1 = poly1[(i + 1) % n1];
                Point a2 = poly2[j], b2 = poly2[(j + 1) % n2];
                
                // Check if segments intersect
                double d1 = cross(a2, b2, a1);
                double d2 = cross(a2, b2, b1);
                double d3 = cross(a1, b1, a2);
                double d4 = cross(a1, b1, b2);
                
                if (((d1 > EPS) != (d2 > EPS)) && ((d3 > EPS) != (d4 > EPS))) {
                    return true;
                }
                
                // Check for collinear overlapping segments
                if (abs(d1) < EPS && abs(d2) < EPS && abs(d3) < EPS && abs(d4) < EPS) {
                    if (max(min(a1.x, b1.x), min(a2.x, b2.x)) <= min(max(a1.x, b1.x), max(a2.x, b2.x)) + EPS &&
                        max(min(a1.y, b1.y), min(a2.y, b2.y)) <= min(max(a1.y, b1.y), max(a2.y, b2.y)) + EPS) {
                        return true;
                    }
                }
            }
        }
        
        return false;
    }
    
    // Minkowski sum of two convex polygons
    Polygon minkowskiSum(const Polygon& poly1, const Polygon& poly2) {
        vector<Point> result;
        
        for (const Point& p1 : poly1) {
            for (const Point& p2 : poly2) {
                result.push_back(p1 + p2);
            }
        }
        
        return convexHull(result);
    }
    
    // Rotate polygon around origin by angle (in radians)
    Polygon rotatePolygon(const Polygon& poly, double angle) {
        Polygon result;
        double cosA = cos(angle), sinA = sin(angle);
        
        for (const Point& p : poly) {
            result.push_back(Point(p.x * cosA - p.y * sinA, p.x * sinA + p.y * cosA));
        }
        
        return result;
    }
    
    // Translate polygon by vector
    Polygon translatePolygon(const Polygon& poly, const Point& translation) {
        Polygon result;
        for (const Point& p : poly) {
            result.push_back(p + translation);
        }
        return result;
    }
    
    // Scale polygon by factor around origin
    Polygon scalePolygon(const Polygon& poly, double factor) {
        Polygon result;
        for (const Point& p : poly) {
            result.push_back(p * factor);
        }
        return result;
    }
    
    // Get bounding box of polygon
    struct BoundingBox {
        double minX, minY, maxX, maxY;
        
        BoundingBox() : minX(1e18), minY(1e18), maxX(-1e18), maxY(-1e18) {}
        
        void addPoint(const Point& p) {
            minX = min(minX, p.x);
            minY = min(minY, p.y);
            maxX = max(maxX, p.x);
            maxY = max(maxY, p.y);
        }
        
        double width() const { return maxX - minX; }
        double height() const { return maxY - minY; }
        double area() const { return width() * height(); }
        Point center() const { return Point((minX + maxX) / 2, (minY + maxY) / 2); }
        
        bool contains(const Point& p) const {
            return p.x >= minX - EPS && p.x <= maxX + EPS &&
                   p.y >= minY - EPS && p.y <= maxY + EPS;
        }
        
        bool intersects(const BoundingBox& other) const {
            return !(maxX < other.minX - EPS || minX > other.maxX + EPS ||
                    maxY < other.minY - EPS || minY > other.maxY + EPS);
        }
    };
    
    BoundingBox getPolygonBoundingBox(const Polygon& poly) {
        BoundingBox bbox;
        for (const Point& p : poly) {
            bbox.addPoint(p);
        }
        return bbox;
    }
    
    // Check if polygon is simple (no self-intersections)
    bool isSimplePolygon(const Polygon& poly) {
        int n = poly.size();
        if (n < 3) return true;
        
        for (int i = 0; i < n; i++) {
            for (int j = i + 2; j < n; j++) {
                if (i == 0 && j == n - 1) continue; // Adjacent edges
                
                Point a1 = poly[i], b1 = poly[(i + 1) % n];
                Point a2 = poly[j], b2 = poly[(j + 1) % n];
                
                // Check if segments intersect
                double d1 = cross(a2, b2, a1);
                double d2 = cross(a2, b2, b1);
                double d3 = cross(a1, b1, a2);
                double d4 = cross(a1, b1, b2);
                
                if (((d1 > EPS) != (d2 > EPS)) && ((d3 > EPS) != (d4 > EPS))) {
                    return false;
                }
            }
        }
        
        return true;
    }
    
    // Get polygon orientation (1 = counter-clockwise, -1 = clockwise, 0 = degenerate)
    int polygonOrientation(const Polygon& poly) {
        double area = polygonArea(poly);
        if (abs(area) < EPS) return 0;
        return (area > 0) ? 1 : -1;
    }
    
    // Reverse polygon orientation
    Polygon reversePolygon(const Polygon& poly) {
        Polygon result = poly;
        reverse(result.begin(), result.end());
        return result;
    }
    
    // Ensure polygon has counter-clockwise orientation
    Polygon ensureCounterClockwise(const Polygon& poly) {
        return (polygonOrientation(poly) < 0) ? reversePolygon(poly) : poly;
    }
    
    // Ensure polygon has clockwise orientation
    Polygon ensureClockwise(const Polygon& poly) {
        return (polygonOrientation(poly) > 0) ? reversePolygon(poly) : poly;
    }
    
    // Find diameter of convex polygon (rotating calipers)
    pair<Point, Point> convexPolygonDiameter(const Polygon& poly) {
        int n = poly.size();
        if (n < 2) return {Point(), Point()};
        if (n == 2) return {poly[0], poly[1]};
        
        double maxDist = 0;
        pair<Point, Point> result;
        
        int j = 1;
        for (int i = 0; i < n; i++) {
            while (true) {
                int next_j = (j + 1) % n;
                Point edge = poly[(i + 1) % n] - poly[i];
                Point vec1 = poly[j] - poly[i];
                Point vec2 = poly[next_j] - poly[i];
                
                if (abs(edge.cross(vec2)) <= abs(edge.cross(vec1))) break;
                j = next_j;
            }
            
            double dist = poly[i].distanceTo(poly[j]);
            if (dist > maxDist) {
                maxDist = dist;
                result = {poly[i], poly[j]};
            }
        }
        
        return result;
    }
}

// Example usage:
/*
int main() {
    using namespace PolygonOperations;
    
    // Create a triangle
    Polygon triangle = {Point(0, 0), Point(4, 0), Point(2, 3)};
    
    cout << "Triangle area: " << polygonAreaAbs(triangle) << endl;
    cout << "Triangle perimeter: " << polygonPerimeter(triangle) << endl;
    cout << "Is convex: " << (isConvex(triangle) ? "Yes" : "No") << endl;
    
    Point centroid = polygonCentroid(triangle);
    cout << "Centroid: (" << centroid.x << ", " << centroid.y << ")" << endl;
    
    // Point in polygon test
    Point testPoint(2, 1);
    cout << "Point (2, 1) is " << (pointInPolygon(testPoint, triangle) ? "inside" : "outside") << " the triangle" << endl;
    
    // Convex hull
    vector<Point> points = {Point(0, 0), Point(1, 1), Point(2, 0), Point(1, 2), Point(0, 2)};
    Polygon hull = convexHull(points);
    cout << "Convex hull has " << hull.size() << " vertices" << endl;
    
    return 0;
}
*/