#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double PI = acos(-1.0);

// Circle operations
namespace CircleOperations {
    
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
        bool operator==(const Point& p) const {
            return abs(x - p.x) < EPS && abs(y - p.y) < EPS;
        }
        double distanceTo(const Point& p) const {
            return (*this - p).norm();
        }
        double distanceToSquared(const Point& p) const {
            return (*this - p).norm2();
        }
        Point normalize() const {
            double n = norm();
            return n > EPS ? Point(x / n, y / n) : Point(0, 0);
        }
        Point rotate(double angle) const {
            double c = cos(angle), s = sin(angle);
            return Point(x * c - y * s, x * s + y * c);
        }
        double angle() const {
            return atan2(y, x);
        }
        static Point polar(double r, double theta) {
            return Point(r * cos(theta), r * sin(theta));
        }
    };
    
    struct Line {
        Point a, b;
        Line() {}
        Line(Point a, Point b) : a(a), b(b) {}
        Point direction() const { return b - a; }
        Point normal() const {
            Point d = direction();
            return Point(-d.y, d.x);
        }
    };
    
    struct Circle {
        Point center;
        double radius;
        
        Circle() : radius(0) {}
        Circle(Point center, double radius) : center(center), radius(radius) {}
        Circle(double x, double y, double r) : center(Point(x, y)), radius(r) {}
        
        bool contains(const Point& p) const {
            return center.distanceTo(p) <= radius + EPS;
        }
        
        bool strictlyContains(const Point& p) const {
            return center.distanceTo(p) < radius - EPS;
        }
        
        bool onBoundary(const Point& p) const {
            return abs(center.distanceTo(p) - radius) < EPS;
        }
        
        double area() const {
            return PI * radius * radius;
        }
        
        double circumference() const {
            return 2 * PI * radius;
        }
        
        Point pointAt(double theta) const {
            return center + Point::polar(radius, theta);
        }
        
        // Get tangent points from external point
        vector<Point> getTangentPoints(const Point& p) const {
            double d = center.distanceTo(p);
            if (d < radius - EPS) return {}; // Point inside circle
            if (abs(d - radius) < EPS) return {p}; // Point on circle
            
            double angle = acos(radius / d);
            Point dir = (p - center).normalize();
            double baseAngle = dir.angle();
            
            return {
                center + Point::polar(radius, baseAngle + angle),
                center + Point::polar(radius, baseAngle - angle)
            };
        }
        
        // Get tangent lines from external point
        vector<Line> getTangentLines(const Point& p) const {
            vector<Point> tangentPoints = getTangentPoints(p);
            vector<Line> result;
            for (const Point& tp : tangentPoints) {
                result.push_back(Line(p, tp));
            }
            return result;
        }
    };
    
    // Create circle from three points
    Circle circumcircle(const Point& a, const Point& b, const Point& c) {
        double d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
        if (abs(d) < EPS) {
            return Circle(); // Collinear points
        }
        
        double ux = ((a.x * a.x + a.y * a.y) * (b.y - c.y) + 
                     (b.x * b.x + b.y * b.y) * (c.y - a.y) + 
                     (c.x * c.x + c.y * c.y) * (a.y - b.y)) / d;
        double uy = ((a.x * a.x + a.y * a.y) * (c.x - b.x) + 
                     (b.x * b.x + b.y * b.y) * (a.x - c.x) + 
                     (c.x * c.x + c.y * c.y) * (b.x - a.x)) / d;
        
        Point center(ux, uy);
        double radius = center.distanceTo(a);
        return Circle(center, radius);
    }
    
    // Circle-circle intersection
    vector<Point> circleCircleIntersection(const Circle& c1, const Circle& c2) {
        double d = c1.center.distanceTo(c2.center);
        
        // No intersection cases
        if (d > c1.radius + c2.radius + EPS) return {}; // Too far apart
        if (d < abs(c1.radius - c2.radius) - EPS) return {}; // One inside the other
        if (abs(d) < EPS && abs(c1.radius - c2.radius) < EPS) {
            // Same circle - infinite intersections
            return {};
        }
        
        // One intersection point (tangent)
        if (abs(d - c1.radius - c2.radius) < EPS || abs(d - abs(c1.radius - c2.radius)) < EPS) {
            Point dir = (c2.center - c1.center).normalize();
            return {c1.center + dir * c1.radius};
        }
        
        // Two intersection points
        double a = (c1.radius * c1.radius - c2.radius * c2.radius + d * d) / (2 * d);
        double h = sqrt(c1.radius * c1.radius - a * a);
        
        Point p = c1.center + (c2.center - c1.center) * (a / d);
        Point offset = (c2.center - c1.center).normalize() * h;
        offset = Point(-offset.y, offset.x); // Perpendicular
        
        return {p + offset, p - offset};
    }
    
    // Line-circle intersection
    vector<Point> lineCircleIntersection(const Line& line, const Circle& circle) {
        Point d = line.direction();
        Point f = line.a - circle.center;
        
        double a = d.dot(d);
        double b = 2 * f.dot(d);
        double c = f.dot(f) - circle.radius * circle.radius;
        
        double discriminant = b * b - 4 * a * c;
        
        if (discriminant < -EPS) return {}; // No intersection
        
        if (abs(discriminant) < EPS) {
            // One intersection (tangent)
            double t = -b / (2 * a);
            return {line.a + d * t};
        }
        
        // Two intersections
        double sqrtD = sqrt(discriminant);
        double t1 = (-b - sqrtD) / (2 * a);
        double t2 = (-b + sqrtD) / (2 * a);
        
        return {line.a + d * t1, line.a + d * t2};
    }
    
    // Check if two circles intersect
    bool circlesIntersect(const Circle& c1, const Circle& c2) {
        double d = c1.center.distanceTo(c2.center);
        return d <= c1.radius + c2.radius + EPS && d >= abs(c1.radius - c2.radius) - EPS;
    }
    
    // Check if circle intersects with line
    bool circleLineIntersect(const Circle& circle, const Line& line) {
        Point d = line.direction();
        Point ac = circle.center - line.a;
        
        double t = ac.dot(d) / d.dot(d);
        Point closest = line.a + d * t;
        
        return circle.center.distanceTo(closest) <= circle.radius + EPS;
    }
    
    // Distance between circle centers
    double circleDistance(const Circle& c1, const Circle& c2) {
        return c1.center.distanceTo(c2.center);
    }
    
    // Check if one circle is inside another
    bool circleInsideCircle(const Circle& inner, const Circle& outer) {
        double d = inner.center.distanceTo(outer.center);
        return d + inner.radius <= outer.radius + EPS;
    }
    
    // Area of intersection of two circles
    double circleIntersectionArea(const Circle& c1, const Circle& c2) {
        double d = c1.center.distanceTo(c2.center);
        
        if (d >= c1.radius + c2.radius - EPS) return 0; // No intersection
        if (d <= abs(c1.radius - c2.radius) + EPS) {
            // One circle inside the other
            double minRadius = min(c1.radius, c2.radius);
            return PI * minRadius * minRadius;
        }
        
        // Partial intersection
        double r1 = c1.radius, r2 = c2.radius;
        double alpha = 2 * acos((d * d + r1 * r1 - r2 * r2) / (2 * d * r1));
        double beta = 2 * acos((d * d + r2 * r2 - r1 * r1) / (2 * d * r2));
        
        double area1 = 0.5 * r1 * r1 * (alpha - sin(alpha));
        double area2 = 0.5 * r2 * r2 * (beta - sin(beta));
        
        return area1 + area2;
    }
    
    // Common tangent lines of two circles
    vector<Line> commonTangents(const Circle& c1, const Circle& c2) {
        vector<Line> result;
        double d = c1.center.distanceTo(c2.center);
        
        if (abs(d) < EPS) return result; // Same center
        
        Point dir = (c2.center - c1.center).normalize();
        Point perp(-dir.y, dir.x);
        
        // External tangents
        if (abs(c1.radius - c2.radius) < EPS) {
            // Parallel external tangents
            Point offset = perp * c1.radius;
            result.push_back(Line(c1.center + offset, c2.center + offset));
            result.push_back(Line(c1.center - offset, c2.center - offset));
        } else {
            // Non-parallel external tangents
            double angle = asin(abs(c1.radius - c2.radius) / d);
            if (c1.radius > c2.radius) angle = -angle;
            
            Point dir1 = dir.rotate(angle);
            Point dir2 = dir.rotate(-angle);
            
            Point p1 = c1.center + Point(-dir1.y, dir1.x) * c1.radius;
            Point q1 = c2.center + Point(-dir1.y, dir1.x) * c2.radius;
            Point p2 = c1.center + Point(dir2.y, -dir2.x) * c1.radius;
            Point q2 = c2.center + Point(dir2.y, -dir2.x) * c2.radius;
            
            result.push_back(Line(p1, q1));
            result.push_back(Line(p2, q2));
        }
        
        // Internal tangents (if circles don't intersect)
        if (d > c1.radius + c2.radius + EPS) {
            double angle = asin((c1.radius + c2.radius) / d);
            
            Point dir1 = dir.rotate(angle);
            Point dir2 = dir.rotate(-angle);
            
            Point p1 = c1.center + Point(-dir1.y, dir1.x) * c1.radius;
            Point q1 = c2.center + Point(dir1.y, -dir1.x) * c2.radius;
            Point p2 = c1.center + Point(dir2.y, -dir2.x) * c1.radius;
            Point q2 = c2.center + Point(-dir2.y, dir2.x) * c2.radius;
            
            result.push_back(Line(p1, q1));
            result.push_back(Line(p2, q2));
        }
        
        return result;
    }
    
    // Smallest circle containing all points
    Circle smallestEnclosingCircle(vector<Point> points) {
        random_shuffle(points.begin(), points.end());
        
        function<Circle(vector<Point>&, int, vector<Point>&)> welzl = 
        [&](vector<Point>& P, int n, vector<Point>& R) -> Circle {
            if (n == 0 || R.size() == 3) {
                if (R.size() == 0) return Circle();
                if (R.size() == 1) return Circle(R[0], 0);
                if (R.size() == 2) {
                    Point center = (R[0] + R[1]) * 0.5;
                    double radius = center.distanceTo(R[0]);
                    return Circle(center, radius);
                }
                return circumcircle(R[0], R[1], R[2]);
            }
            
            Circle circle = welzl(P, n - 1, R);
            
            if (circle.contains(P[n - 1])) {
                return circle;
            }
            
            R.push_back(P[n - 1]);
            circle = welzl(P, n - 1, R);
            R.pop_back();
            
            return circle;
        };
        
        vector<Point> R;
        return welzl(points, points.size(), R);
    }
    
    // Circle through two points with given radius
    vector<Circle> circlesThroughTwoPoints(const Point& p1, const Point& p2, double radius) {
        double d = p1.distanceTo(p2);
        
        if (d > 2 * radius + EPS) return {}; // Impossible
        if (abs(d) < EPS) return {}; // Same point
        
        Point mid = (p1 + p2) * 0.5;
        
        if (abs(d - 2 * radius) < EPS) {
            // One circle (diameter)
            return {Circle(mid, radius)};
        }
        
        // Two circles
        double h = sqrt(radius * radius - (d / 2) * (d / 2));
        Point dir = (p2 - p1).normalize();
        Point perp(-dir.y, dir.x);
        
        return {
            Circle(mid + perp * h, radius),
            Circle(mid - perp * h, radius)
        };
    }
    
    // Inversion of point with respect to circle
    Point invertPoint(const Point& p, const Circle& circle) {
        Point dir = p - circle.center;
        double dist = dir.norm();
        
        if (abs(dist) < EPS) return Point(1e18, 1e18); // Invalid
        
        double newDist = circle.radius * circle.radius / dist;
        return circle.center + dir.normalize() * newDist;
    }
    
    // Power of point with respect to circle
    double powerOfPoint(const Point& p, const Circle& circle) {
        double d = circle.center.distanceTo(p);
        return d * d - circle.radius * circle.radius;
    }
    
    // Radical axis of two circles
    Line radicalAxis(const Circle& c1, const Circle& c2) {
        Point diff = c2.center - c1.center;
        double d2 = diff.norm2();
        
        if (abs(d2) < EPS) {
            // Same center - no radical axis
            return Line();
        }
        
        double k = (c1.radius * c1.radius - c2.radius * c2.radius) / d2;
        Point mid = (c1.center + c2.center) * 0.5 + diff * (k * 0.5);
        Point perp(-diff.y, diff.x);
        
        return Line(mid, mid + perp);
    }
    
    // Check if four points are concyclic
    bool areConcyclic(const Point& a, const Point& b, const Point& c, const Point& d) {
        Circle circle = circumcircle(a, b, c);
        return circle.onBoundary(d);
    }
    
    // Apollonius circle (locus of points with constant ratio of distances to two points)
    Circle apolloniusCircle(const Point& a, const Point& b, double ratio) {
        if (abs(ratio - 1.0) < EPS) {
            // Perpendicular bisector (infinite radius)
            return Circle();
        }
        
        double k = ratio * ratio;
        Point center = (a - b * k) / (1 - k);
        double radius = abs(ratio) * a.distanceTo(b) / abs(1 - k);
        
        return Circle(center, radius);
    }
}

// Example usage:
/*
int main() {
    using namespace CircleOperations;
    
    // Create circles
    Circle c1(Point(0, 0), 3);
    Circle c2(Point(4, 0), 2);
    
    cout << "Circle 1: center (0, 0), radius 3" << endl;
    cout << "Circle 2: center (4, 0), radius 2" << endl;
    
    // Check intersection
    vector<Point> intersections = circleCircleIntersection(c1, c2);
    cout << "Number of intersections: " << intersections.size() << endl;
    for (const Point& p : intersections) {
        cout << "Intersection at: (" << p.x << ", " << p.y << ")" << endl;
    }
    
    // Intersection area
    double area = circleIntersectionArea(c1, c2);
    cout << "Intersection area: " << area << endl;
    
    // Common tangents
    vector<Line> tangents = commonTangents(c1, c2);
    cout << "Number of common tangents: " << tangents.size() << endl;
    
    // Circumcircle of three points
    Point a(0, 0), b(4, 0), c(2, 3);
    Circle circumcircle_abc = circumcircle(a, b, c);
    cout << "Circumcircle center: (" << circumcircle_abc.center.x << ", " << circumcircle_abc.center.y << ")" << endl;
    cout << "Circumcircle radius: " << circumcircle_abc.radius << endl;
    
    // Line-circle intersection
    Line line(Point(-1, 0), Point(1, 0));
    vector<Point> lineIntersections = lineCircleIntersection(line, c1);
    cout << "Line intersects circle at " << lineIntersections.size() << " points" << endl;
    
    return 0;
}
*/