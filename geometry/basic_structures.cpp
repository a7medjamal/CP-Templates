#include <bits/stdc++.h>
using namespace std;

const double EPS = 1e-9;
const double PI = acos(-1.0);

// Basic geometric structures and operations
namespace BasicGeometry {
    
    // Point structure
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
        
        Point normalize() const {
            double n = norm();
            return n > EPS ? Point(x / n, y / n) : Point(0, 0);
        }
        
        Point rotate(double angle) const {
            double c = cos(angle), s = sin(angle);
            return Point(x * c - y * s, x * s + y * c);
        }
        
        Point perpendicular() const {
            return Point(-y, x);
        }
        
        bool operator<(const Point& p) const {
            return x < p.x || (abs(x - p.x) < EPS && y < p.y);
        }
        
        bool operator==(const Point& p) const {
            return abs(x - p.x) < EPS && abs(y - p.y) < EPS;
        }
        
        bool operator!=(const Point& p) const {
            return !(*this == p);
        }
        
        // Polar coordinates
        double angle() const {
            return atan2(y, x);
        }
        
        static Point polar(double r, double theta) {
            return Point(r * cos(theta), r * sin(theta));
        }
        
        // Distance to another point
        double distanceTo(const Point& p) const {
            return (*this - p).norm();
        }
        
        double distanceToSquared(const Point& p) const {
            return (*this - p).norm2();
        }
    };
    
    typedef Point Vector;
    
    // Line structure
    struct Line {
        Point a, b;
        
        Line() {}
        Line(Point a, Point b) : a(a), b(b) {}
        Line(double x1, double y1, double x2, double y2) : a(Point(x1, y1)), b(Point(x2, y2)) {}
        
        Vector direction() const { return b - a; }
        Vector normal() const {
            Vector d = direction();
            return Vector(-d.y, d.x);
        }
        
        double length() const {
            return direction().norm();
        }
        
        // Get point at parameter t (0 <= t <= 1 for segment)
        Point pointAt(double t) const {
            return a + direction() * t;
        }
        
        // Check if line is vertical
        bool isVertical() const {
            return abs(b.x - a.x) < EPS;
        }
        
        // Check if line is horizontal
        bool isHorizontal() const {
            return abs(b.y - a.y) < EPS;
        }
        
        // Get slope (undefined for vertical lines)
        double slope() const {
            if (isVertical()) return 1e18; // Large number to represent infinity
            return (b.y - a.y) / (b.x - a.x);
        }
        
        // Get y-intercept (for non-vertical lines)
        double yIntercept() const {
            if (isVertical()) return 1e18;
            return a.y - slope() * a.x;
        }
    };
    
    // Circle structure
    struct Circle {
        Point center;
        double radius;
        
        Circle() : radius(0) {}
        Circle(Point center, double radius) : center(center), radius(radius) {}
        Circle(double x, double y, double r) : center(Point(x, y)), radius(r) {}
        
        bool contains(const Point& p) const {
            return (p - center).norm() <= radius + EPS;
        }
        
        bool strictlyContains(const Point& p) const {
            return (p - center).norm() < radius - EPS;
        }
        
        bool onBoundary(const Point& p) const {
            return abs((p - center).norm() - radius) < EPS;
        }
        
        double area() const {
            return PI * radius * radius;
        }
        
        double circumference() const {
            return 2 * PI * radius;
        }
        
        // Get point on circle at angle theta
        Point pointAt(double theta) const {
            return center + Point::polar(radius, theta);
        }
        
        // Check if two circles intersect
        bool intersects(const Circle& other) const {
            double d = center.distanceTo(other.center);
            return d <= radius + other.radius + EPS && d >= abs(radius - other.radius) - EPS;
        }
        
        // Check if this circle is inside another
        bool isInside(const Circle& other) const {
            double d = center.distanceTo(other.center);
            return d + radius <= other.radius + EPS;
        }
    };
    
    // Rectangle structure
    struct Rectangle {
        Point bottomLeft, topRight;
        
        Rectangle() {}
        Rectangle(Point bl, Point tr) : bottomLeft(bl), topRight(tr) {}
        Rectangle(double x1, double y1, double x2, double y2) {
            bottomLeft = Point(min(x1, x2), min(y1, y2));
            topRight = Point(max(x1, x2), max(y1, y2));
        }
        
        double width() const { return topRight.x - bottomLeft.x; }
        double height() const { return topRight.y - bottomLeft.y; }
        double area() const { return width() * height(); }
        double perimeter() const { return 2 * (width() + height()); }
        
        Point center() const {
            return Point((bottomLeft.x + topRight.x) / 2, (bottomLeft.y + topRight.y) / 2);
        }
        
        bool contains(const Point& p) const {
            return p.x >= bottomLeft.x - EPS && p.x <= topRight.x + EPS &&
                   p.y >= bottomLeft.y - EPS && p.y <= topRight.y + EPS;
        }
        
        bool intersects(const Rectangle& other) const {
            return !(topRight.x < other.bottomLeft.x - EPS || 
                    bottomLeft.x > other.topRight.x + EPS ||
                    topRight.y < other.bottomLeft.y - EPS || 
                    bottomLeft.y > other.topRight.y + EPS);
        }
        
        Rectangle intersection(const Rectangle& other) const {
            if (!intersects(other)) return Rectangle();
            
            return Rectangle(
                max(bottomLeft.x, other.bottomLeft.x),
                max(bottomLeft.y, other.bottomLeft.y),
                min(topRight.x, other.topRight.x),
                min(topRight.y, other.topRight.y)
            );
        }
        
        vector<Point> getVertices() const {
            return {
                bottomLeft,
                Point(topRight.x, bottomLeft.y),
                topRight,
                Point(bottomLeft.x, topRight.y)
            };
        }
    };
    
    // Triangle structure
    struct Triangle {
        Point a, b, c;
        
        Triangle() {}
        Triangle(Point a, Point b, Point c) : a(a), b(b), c(c) {}
        
        double area() const {
            return abs((b - a).cross(c - a)) / 2.0;
        }
        
        double perimeter() const {
            return a.distanceTo(b) + b.distanceTo(c) + c.distanceTo(a);
        }
        
        Point centroid() const {
            return (a + b + c) / 3.0;
        }
        
        Point circumcenter() const {
            double d = 2 * (a.x * (b.y - c.y) + b.x * (c.y - a.y) + c.x * (a.y - b.y));
            if (abs(d) < EPS) return Point(); // Collinear points
            
            double ux = ((a.x * a.x + a.y * a.y) * (b.y - c.y) + 
                         (b.x * b.x + b.y * b.y) * (c.y - a.y) + 
                         (c.x * c.x + c.y * c.y) * (a.y - b.y)) / d;
            double uy = ((a.x * a.x + a.y * a.y) * (c.x - b.x) + 
                         (b.x * b.x + b.y * b.y) * (a.x - c.x) + 
                         (c.x * c.x + c.y * c.y) * (b.x - a.x)) / d;
            
            return Point(ux, uy);
        }
        
        Circle circumcircle() const {
            Point center = circumcenter();
            double radius = center.distanceTo(a);
            return Circle(center, radius);
        }
        
        Point incenter() const {
            double sideA = b.distanceTo(c);
            double sideB = c.distanceTo(a);
            double sideC = a.distanceTo(b);
            double perimeter = sideA + sideB + sideC;
            
            return (a * sideA + b * sideB + c * sideC) / perimeter;
        }
        
        Circle incircle() const {
            Point center = incenter();
            double sideA = b.distanceTo(c);
            double sideB = c.distanceTo(a);
            double sideC = a.distanceTo(b);
            double s = (sideA + sideB + sideC) / 2.0;
            double radius = area() / s;
            return Circle(center, radius);
        }
        
        bool contains(const Point& p) const {
            // Using barycentric coordinates
            double denom = (b.y - c.y) * (a.x - c.x) + (c.x - b.x) * (a.y - c.y);
            if (abs(denom) < EPS) return false; // Degenerate triangle
            
            double alpha = ((b.y - c.y) * (p.x - c.x) + (c.x - b.x) * (p.y - c.y)) / denom;
            double beta = ((c.y - a.y) * (p.x - c.x) + (a.x - c.x) * (p.y - c.y)) / denom;
            double gamma = 1 - alpha - beta;
            
            return alpha >= -EPS && beta >= -EPS && gamma >= -EPS;
        }
        
        bool isDegenerate() const {
            return abs((b - a).cross(c - a)) < EPS;
        }
        
        // Check if triangle is right-angled
        bool isRightAngled() const {
            double a2 = b.distanceToSquared(c);
            double b2 = c.distanceToSquared(a);
            double c2 = a.distanceToSquared(b);
            
            return abs(a2 + b2 - c2) < EPS || 
                   abs(b2 + c2 - a2) < EPS || 
                   abs(c2 + a2 - b2) < EPS;
        }
        
        // Get angles in radians
        vector<double> getAngles() const {
            double sideA = b.distanceTo(c);
            double sideB = c.distanceTo(a);
            double sideC = a.distanceTo(b);
            
            double angleA = acos((sideB * sideB + sideC * sideC - sideA * sideA) / (2 * sideB * sideC));
            double angleB = acos((sideA * sideA + sideC * sideC - sideB * sideB) / (2 * sideA * sideC));
            double angleC = PI - angleA - angleB;
            
            return {angleA, angleB, angleC};
        }
    };
    
    // Basic utility functions
    double distance(const Point& a, const Point& b) {
        return (a - b).norm();
    }
    
    double distanceSquared(const Point& a, const Point& b) {
        return (a - b).norm2();
    }
    
    // Cross product of vectors OA and OB
    double cross(const Point& O, const Point& A, const Point& B) {
        return (A - O).cross(B - O);
    }
    
    // Dot product of vectors OA and OB
    double dot(const Point& O, const Point& A, const Point& B) {
        return (A - O).dot(B - O);
    }
    
    // Check if three points are collinear
    bool collinear(const Point& a, const Point& b, const Point& c) {
        return abs(cross(a, b, c)) < EPS;
    }
    
    // Check orientation of three points
    // Returns: 0 = collinear, 1 = clockwise, 2 = counterclockwise
    int orientation(const Point& a, const Point& b, const Point& c) {
        double val = cross(a, b, c);
        if (abs(val) < EPS) return 0;
        return (val > 0) ? 2 : 1;
    }
    
    // Check if point c is on segment ab
    bool onSegment(const Point& a, const Point& b, const Point& c) {
        if (!collinear(a, b, c)) return false;
        return min(a.x, b.x) <= c.x + EPS && c.x <= max(a.x, b.x) + EPS &&
               min(a.y, b.y) <= c.y + EPS && c.y <= max(a.y, b.y) + EPS;
    }
    
    // Angle between three points (in radians)
    double angle(const Point& a, const Point& b, const Point& c) {
        Vector ba = a - b;
        Vector bc = c - b;
        return acos(ba.dot(bc) / (ba.norm() * bc.norm()));
    }
    
    // Signed angle from vector u to vector v (in radians)
    double signedAngle(const Vector& u, const Vector& v) {
        return atan2(u.cross(v), u.dot(v));
    }
}

// Example usage:
/*
int main() {
    using namespace BasicGeometry;
    
    // Points
    Point a(0, 0), b(3, 4), c(1, 1);
    cout << "Distance from (0,0) to (3,4): " << distance(a, b) << endl;
    cout << "Are points collinear? " << (collinear(a, b, c) ? "Yes" : "No") << endl;
    
    // Circle
    Circle circle(Point(0, 0), 5);
    cout << "Point (3, 4) is " << (circle.contains(Point(3, 4)) ? "inside" : "outside") << " the circle" << endl;
    cout << "Circle area: " << circle.area() << endl;
    
    // Triangle
    Triangle triangle(Point(0, 0), Point(4, 0), Point(2, 3));
    cout << "Triangle area: " << triangle.area() << endl;
    cout << "Triangle centroid: (" << triangle.centroid().x << ", " << triangle.centroid().y << ")" << endl;
    
    // Rectangle
    Rectangle rect(0, 0, 4, 3);
    cout << "Rectangle area: " << rect.area() << endl;
    cout << "Rectangle contains (2, 1.5): " << (rect.contains(Point(2, 1.5)) ? "Yes" : "No") << endl;
    
    return 0;
}
*/