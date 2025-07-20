#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double PI = acos(-1.0);

// Line and segment operations
namespace LineOperations {
    
    struct Point {
        double x, y;
        Point() : x(0), y(0) {}
        Point(double x, double y) : x(x), y(y) {}
        Point operator+(const Point& p) const { return Point(x + p.x, y + p.y); }
        Point operator-(const Point& p) const { return Point(x - p.x, y - p.y); }
        Point operator*(double t) const { return Point(x * t, y * t); }
        double dot(const Point& p) const { return x * p.x + y * p.y; }
        double cross(const Point& p) const { return x * p.y - y * p.x; }
        double norm() const { return sqrt(x * x + y * y); }
        double norm2() const { return x * x + y * y; }
        Point normalize() const {
            double n = norm();
            return n > EPS ? Point(x / n, y / n) : Point(0, 0);
        }
        double distanceTo(const Point& p) const {
            return (*this - p).norm();
        }
        bool operator==(const Point& p) const {
            return abs(x - p.x) < EPS && abs(y - p.y) < EPS;
        }
    };
    
    typedef Point Vector;
    
    struct Line {
        Point a, b;
        Line() {}
        Line(Point a, Point b) : a(a), b(b) {}
        Vector direction() const { return b - a; }
        Vector normal() const {
            Vector d = direction();
            return Vector(-d.y, d.x);
        }
    };
    
    struct Segment {
        Point a, b;
        Segment() {}
        Segment(Point a, Point b) : a(a), b(b) {}
        Vector direction() const { return b - a; }
        double length() const { return direction().norm(); }
        Point midpoint() const { return (a + b) * 0.5; }
    };
    
    // Cross product of vectors OA and OB
    double cross(const Point& O, const Point& A, const Point& B) {
        return (A - O).cross(B - O);
    }
    
    // Check if three points are collinear
    bool collinear(const Point& a, const Point& b, const Point& c) {
        return abs(cross(a, b, c)) < EPS;
    }
    
    // Check if point c is on segment ab
    bool onSegment(const Point& a, const Point& b, const Point& c) {
        if (!collinear(a, b, c)) return false;
        return min(a.x, b.x) <= c.x + EPS && c.x <= max(a.x, b.x) + EPS &&
               min(a.y, b.y) <= c.y + EPS && c.y <= max(a.y, b.y) + EPS;
    }
    
    // Line intersection
    // Returns intersection point of two lines (assumes they intersect)
    Point lineIntersection(const Line& l1, const Line& l2) {
        Vector d1 = l1.direction();
        Vector d2 = l2.direction();
        double det = d1.cross(d2);
        
        if (abs(det) < EPS) {
            // Lines are parallel
            return Point(1e18, 1e18); // Invalid point
        }
        
        Vector diff = l2.a - l1.a;
        double t = diff.cross(d2) / det;
        return l1.a + d1 * t;
    }
    
    // Check if two lines are parallel
    bool areParallel(const Line& l1, const Line& l2) {
        return abs(l1.direction().cross(l2.direction())) < EPS;
    }
    
    // Check if two lines are the same
    bool areSameLine(const Line& l1, const Line& l2) {
        return areParallel(l1, l2) && collinear(l1.a, l1.b, l2.a);
    }
    
    // Segment intersection
    // Returns: 0 = no intersection, 1 = intersection at one point, 2 = overlap
    int segmentIntersection(const Segment& s1, const Segment& s2, Point& intersection) {
        Vector d1 = s1.direction();
        Vector d2 = s2.direction();
        Vector diff = s2.a - s1.a;
        
        double det = d1.cross(d2);
        
        if (abs(det) < EPS) {
            // Segments are parallel
            if (!collinear(s1.a, s1.b, s2.a)) {
                return 0; // No intersection
            }
            
            // Check for overlap
            double t0 = 0, t1 = 1;
            double u0 = diff.dot(d1) / d1.norm2();
            double u1 = (diff + d2).dot(d1) / d1.norm2();
            
            if (u0 > u1) swap(u0, u1);
            
            double left = max(t0, u0);
            double right = min(t1, u1);
            
            if (left <= right + EPS) {
                intersection = s1.a + d1 * left;
                return (abs(left - right) < EPS) ? 1 : 2;
            }
            return 0;
        }
        
        double t = diff.cross(d2) / det;
        double u = diff.cross(d1) / det;
        
        if (t >= -EPS && t <= 1 + EPS && u >= -EPS && u <= 1 + EPS) {
            intersection = s1.a + d1 * t;
            return 1;
        }
        
        return 0;
    }
    
    // Check if two segments intersect
    bool segmentsIntersect(const Segment& s1, const Segment& s2) {
        Point dummy;
        return segmentIntersection(s1, s2, dummy) > 0;
    }
    
    // Distance from point to line
    double pointToLineDistance(const Point& p, const Line& line) {
        Vector d = line.direction();
        Vector ap = p - line.a;
        return abs(ap.cross(d)) / d.norm();
    }
    
    // Distance from point to segment
    double pointToSegmentDistance(const Point& p, const Segment& seg) {
        Vector d = seg.direction();
        Vector ap = p - seg.a;
        
        if (d.norm2() < EPS) {
            // Degenerate segment (point)
            return ap.norm();
        }
        
        double t = ap.dot(d) / d.norm2();
        
        if (t < 0) {
            return (p - seg.a).norm();
        } else if (t > 1) {
            return (p - seg.b).norm();
        } else {
            Point projection = seg.a + d * t;
            return (p - projection).norm();
        }
    }
    
    // Project point onto line
    Point projectPointOnLine(const Point& p, const Line& line) {
        Vector d = line.direction();
        Vector ap = p - line.a;
        double t = ap.dot(d) / d.norm2();
        return line.a + d * t;
    }
    
    // Project point onto segment
    Point projectPointOnSegment(const Point& p, const Segment& seg) {
        Vector d = seg.direction();
        Vector ap = p - seg.a;
        
        if (d.norm2() < EPS) {
            return seg.a; // Degenerate segment
        }
        
        double t = ap.dot(d) / d.norm2();
        t = max(0.0, min(1.0, t)); // Clamp to [0, 1]
        return seg.a + d * t;
    }
    
    // Reflect point across line
    Point reflectPointAcrossLine(const Point& p, const Line& line) {
        Point projection = projectPointOnLine(p, line);
        return projection * 2 - p;
    }
    
    // Get perpendicular line through point
    Line perpendicularLineThrough(const Point& p, const Line& line) {
        Vector normal = line.normal().normalize();
        return Line(p, p + normal);
    }
    
    // Get parallel line through point
    Line parallelLineThrough(const Point& p, const Line& line) {
        Vector direction = line.direction();
        return Line(p, p + direction);
    }
    
    // Distance between two parallel lines
    double distanceBetweenParallelLines(const Line& l1, const Line& l2) {
        if (!areParallel(l1, l2)) {
            return 0; // Lines are not parallel
        }
        return pointToLineDistance(l1.a, l2);
    }
    
    // Distance between two segments
    double segmentToSegmentDistance(const Segment& s1, const Segment& s2) {
        Point intersection;
        if (segmentIntersection(s1, s2, intersection) > 0) {
            return 0; // Segments intersect
        }
        
        // Check all endpoint-to-segment distances
        double minDist = 1e18;
        minDist = min(minDist, pointToSegmentDistance(s1.a, s2));
        minDist = min(minDist, pointToSegmentDistance(s1.b, s2));
        minDist = min(minDist, pointToSegmentDistance(s2.a, s1));
        minDist = min(minDist, pointToSegmentDistance(s2.b, s1));
        
        return minDist;
    }
    
    // Check if point is on the left side of directed line
    bool isLeftOfLine(const Point& p, const Line& line) {
        return cross(line.a, line.b, p) > EPS;
    }
    
    // Check if point is on the right side of directed line
    bool isRightOfLine(const Point& p, const Line& line) {
        return cross(line.a, line.b, p) < -EPS;
    }
    
    // Get line equation in form ax + by + c = 0
    struct LineEquation {
        double a, b, c;
        
        LineEquation(const Line& line) {
            Vector d = line.direction();
            a = -d.y;
            b = d.x;
            c = -(a * line.a.x + b * line.a.y);
            
            // Normalize
            double norm = sqrt(a * a + b * b);
            if (norm > EPS) {
                a /= norm;
                b /= norm;
                c /= norm;
            }
        }
        
        double evaluate(const Point& p) const {
            return a * p.x + b * p.y + c;
        }
        
        double distanceToPoint(const Point& p) const {
            return abs(evaluate(p));
        }
    };
    
    // Angle between two lines (in radians, 0 to PI/2)
    double angleBetweenLines(const Line& l1, const Line& l2) {
        Vector d1 = l1.direction().normalize();
        Vector d2 = l2.direction().normalize();
        double cosTheta = abs(d1.dot(d2));
        return acos(min(1.0, cosTheta));
    }
    
    // Bisector of angle formed by two lines
    vector<Line> angleBisectors(const Line& l1, const Line& l2) {
        Point intersection = lineIntersection(l1, l2);
        if (intersection.x > 1e17) {
            // Lines are parallel, no bisectors
            return {};
        }
        
        Vector d1 = l1.direction().normalize();
        Vector d2 = l2.direction().normalize();
        
        Vector bisector1 = d1 + d2;
        Vector bisector2 = d1 - d2;
        
        vector<Line> result;
        if (bisector1.norm() > EPS) {
            result.push_back(Line(intersection, intersection + bisector1));
        }
        if (bisector2.norm() > EPS) {
            result.push_back(Line(intersection, intersection + bisector2));
        }
        
        return result;
    }
    
    // Find foot of perpendicular from point to line
    Point footOfPerpendicular(const Point& p, const Line& line) {
        return projectPointOnLine(p, line);
    }
    
    // Check if four points form a rectangle
    bool isRectangle(const Point& a, const Point& b, const Point& c, const Point& d) {
        vector<Point> points = {a, b, c, d};
        
        // Calculate all distances
        vector<double> distances;
        for (int i = 0; i < 4; i++) {
            for (int j = i + 1; j < 4; j++) {
                distances.push_back(points[i].distanceTo(points[j]));
            }
        }
        
        sort(distances.begin(), distances.end());
        
        // For a rectangle: 4 equal sides and 2 equal diagonals
        return abs(distances[0] - distances[1]) < EPS &&
               abs(distances[1] - distances[2]) < EPS &&
               abs(distances[2] - distances[3]) < EPS &&
               abs(distances[4] - distances[5]) < EPS &&
               distances[4] > distances[3] + EPS;
    }
    
    // Check if four points form a parallelogram
    bool isParallelogram(const Point& a, const Point& b, const Point& c, const Point& d) {
        // Check if opposite sides are parallel and equal
        Vector ab = b - a;
        Vector dc = c - d;
        Vector ad = d - a;
        Vector bc = c - b;
        
        return abs(ab.cross(dc)) < EPS && abs(ab.norm() - dc.norm()) < EPS &&
               abs(ad.cross(bc)) < EPS && abs(ad.norm() - bc.norm()) < EPS;
    }
}

// Example usage:
/*
int main() {
    using namespace LineOperations;
    
    // Line intersection
    Line l1(Point(0, 0), Point(2, 2));
    Line l2(Point(0, 2), Point(2, 0));
    Point intersection = lineIntersection(l1, l2);
    cout << "Lines intersect at: (" << intersection.x << ", " << intersection.y << ")" << endl;
    
    // Segment intersection
    Segment s1(Point(0, 0), Point(2, 2));
    Segment s2(Point(0, 2), Point(2, 0));
    Point segIntersection;
    int result = segmentIntersection(s1, s2, segIntersection);
    if (result > 0) {
        cout << "Segments intersect at: (" << segIntersection.x << ", " << segIntersection.y << ")" << endl;
    }
    
    // Distance calculations
    Point p(1, 3);
    cout << "Distance from point to line: " << pointToLineDistance(p, l1) << endl;
    cout << "Distance from point to segment: " << pointToSegmentDistance(p, s1) << endl;
    
    // Projection
    Point projected = projectPointOnLine(p, l1);
    cout << "Projected point: (" << projected.x << ", " << projected.y << ")" << endl;
    
    return 0;
}
*/