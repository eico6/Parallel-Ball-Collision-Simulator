// #include "vector.h"

// // constructors
// vector::vector(double x, double y) : x(x), y(y) {}
// vector::vector(const vector& v) : x(v.x), y(v.y) {}

// // modifiers
// vector& vector::set(double x_, double y_) {
//     x = x_; y = y_;
//     return *this;
// }

// vector& vector::add(double dx, double dy) {
//     x += dx; y += dy;
//     return *this;
// }

// vector& vector::add(const vector& v) {
//     x += v.x; y += v.y;
//     return *this;
// }

// vector& vector::sub(double dx, double dy) {
//     x -= dx; y -= dy;
//     return *this;
// }

// vector& vector::sub(const vector& v) {
//     x -= v.x; y -= v.y;
//     return *this;
// }

// vector& vector::scale(double s) {
//     x *= s; y *= s;
//     return *this;
// }

// vector& vector::nor() {
//     double l = len();
//     x /= l; y /= l;
//     return *this;
// }

// vector& vector::rotateDeg(double degrees) {
//     double rad = degrees * 3.1415 / 180.0; // TODO: pi accuracy
//     double c = cos(rad), s = sin(rad);
//     return set(x*c - y*s, x*s + y*c);
// }

// vector& vector::reverse() {
//     x *= -1; y *= -1;
//     return *this;
// }

// // length
// double vector::len(double x, double y) {
//     return std::sqrt(x*x + y*y);
// }

// double vector::len() const {
//     return std::sqrt(x*x + y*y);
// }

// double vector::len2(double x, double y) {
//     return x*x + y*y;
// }

// double vector::len2() const {
//     return x*x + y*y;
// }

// // distance
// double vector::distance(double x1, double y1, double x2, double y2) {
//     double dx = x2 - x1, dy = y2 - y1;
//     return std::sqrt(dx*dx + dy*dy);
// }

// double vector::distance(const vector& v) const {
//     double dx = v.x - x, dy = v.y - y;
//     return std::sqrt(dx*dx + dy*dy);
// }

// double vector::distance2(const vector& v) const {
//     double dx = v.x - x, dy = v.y - y;
//     return dx*dx + dy*dy;
// }

// // dot
// double vector::dot(double x1, double y1, double x2, double y2) {
//     return x1*x2 + y1*y2;
// }

// double vector::dot(double x_, double y_) const {
//     return x*x_ + y*y_;
// }

// double vector::dot(const vector& v) const {
//     return x*v.x + y*v.y;
// }

// // cross
// double vector::cross(double x1, double y1, double x2, double y2) {
//     return x1*y2 - y1*x2;
// }

// double vector::cross(double x_, double y_) const {
//     return x*y_ - y*x_;
// }

// double vector::cross(const vector& v) const {
//     return x*v.y - v.x*y;
// }

// // utils
// bool vector::operator==(const vector& v) const {
//     return x == v.x && y == v.y;
// }

// std::string vector::toString() const {
//     return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
// }