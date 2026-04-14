// #ifndef VECTOR_H
// #define VECTOR_H

// #include <cmath>
// #include <string>

// struct vector {
//     double x, y;

//     // constructors
//     vector(double x, double y);
//     vector(const vector& v);

//     // modifiers (return *this for chaining)
//     vector& set(double x, double y);

//     vector& add(double dx, double dy);
//     vector& add(const vector& v);

//     vector& sub(double x, double y);
//     vector& sub(const vector& v);

//     vector& scale(double scalar);
//     vector& nor();
//     vector& rotateDeg(double degrees);
//     vector& reverse();

//     // length
//     static double len(double x, double y);
//     double len() const;

//     static double len2(double x, double y);
//     double len2() const;

//     // distance
//     static double distance(double x1, double y1, double x2, double y2);
//     double distance(const vector& v) const;
//     double distance2(const vector& v) const;

//     // dot
//     static double dot(double x1, double y1, double x2, double y2);
//     double dot(double x, double y) const;
//     double dot(const vector& v) const;

//     // cross
//     static double cross(double x1, double y1, double x2, double y2);
//     double cross(double x, double y) const;
//     double cross(const vector& v) const;

//     // utils
//     bool operator==(const vector& v) const;
//     std::string toString() const;
// };

// #endif