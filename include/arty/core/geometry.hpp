#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include <arty/core/math.h>

#include <variant>

namespace arty {

template <typename T>
struct Intersection {
  bool exist;
  T value;

  Intersection() : exist(false), value() {}
};

template <typename T, int Dim>
class Line {
 public:
  Line(Vec<T, Dim> const& a, Vec<T, Dim> const& b) : _ori(a), _dir(b - a) {
    _normSquared = _dir.normsqr();
  }

  Vec<T, Dim> project(Vec<T, Dim> const& p) {
    return _ori + _dir * dirCoeff(p);
  }

  T distanceSquaredTo(Vec<T, Dim> const& p) {
    return (project(p) - p).normsqr();
  }

  T distanceTo(Vec<T, Dim> const& p) { return std::sqrt(distanceSquaredTo(p)); }

  T dirCoeff(Vec<T, Dim> const& p) {
    return (p - _ori).dot(_dir) / _normSquared;
  }

  Vec<T, Dim> const& origin() const { return _ori; }
  Vec<T, Dim> const& direction() const { return _dir; }

 private:
  Vec<T, Dim> _ori;
  Vec<T, Dim> _dir;
  T _normSquared;
};

template <typename T>
using Line3 = Line<T, 3>;
using Line3f = Line3<float>;

template <typename T, int Dim>
class Edge {
 public:
  Edge(Vec<T, Dim> const& a, Vec<T, Dim> const& b) : _a(a), _b(b) {}

  Vec<T, Dim> project(Vec<T, Dim> const& p) {
    Line<T, Dim> line(_a, _b);
    T coeff = line.dirCoeff(p);
    if (coeff <= 0) {
      return _a;
    }
    if (coeff >= 1.f) {
      return _b;
    }
    return _a + line.direction() * coeff;
  }

  T distanceSquaredTo(Vec<T, Dim> const& p) {
    return (project(p) - p).normsqr();
  }

  T distanceTo(Vec<T, Dim> const& p) { return std::sqrt(distanceSquaredTo(p)); }

  Vec<T, Dim> const& p1() const { return _a; }
  Vec<T, Dim> const& p2() const { return _b; }

 private:
  Vec<T, Dim> _a;
  Vec<T, Dim> _b;
};

template <typename T>
using Edge3 = Edge<T, 3>;
using Edge3f = Edge3<float>;

template <typename T>
class Plane {
 public:
  Plane(Vec3<T> const& pt1, Vec3<T> const& pt2, Vec3<T> const& pt3)
      : _line(pt1, cross(pt2 - pt1, pt3 - pt1)) {}

  Vec3<T> project(Vec3<T> const& p) {
    T coeff = _line.dirCoeff(p);
    return p - _line.direction() * coeff;
  }

  T distanceSquaredTo(Vec3<T> const& p) { return (project(p) - p).normsqr(); }

  T distanceTo(Vec3<T> const& p) { return std::sqrt(distanceSquaredTo(p)); }

  Vec3<T> const& origin() const { return _line.origin(); }
  Vec3<T> direction() const { return _line.direction(); }

  T sideOf(Vec3<T> const& p) {
    T coeff = _line.dirCoeff(p);
    return (T(0) < coeff) - (T(0) > coeff);
  }

 private:
  Line3<T> _line;
};
using Plane3f = Plane<float>;

template <typename T>
class Triangle {
 public:
  Triangle(Vec3<T> const& p1, Vec3<T> const& p2, Vec3<T> const& p3)
      : _p1(p1), _p2(p2), _p3(p3) {}

  Vec3<T> project(Vec3<T> const& p) {
    Edge3<T> e0(_p1, _p2);
    Vec3<T> proj0 = e0.project(p);
    T dist0 = (proj0 - p).normsqr();
    Edge3<T> e1(_p2, _p3);
    Vec3<T> proj1 = e1.project(p);
    T dist1 = (proj1 - p).normsqr();
    Edge3<T> e2(_p3, _p1);
    Vec3<T> proj2 = e2.project(p);
    T dist2 = (proj2 - p).normsqr();
    Plane<T> e3(_p1, _p2, _p3);
    Vec3<T> proj3 = e3.project(p);
    T dist3 = (proj3 - p).normsqr();
    std::vector<T> el({dist0, dist1, dist2, dist3});
    std::size_t argmin = std::distance(
        std::min_element(std::begin(el), std::end(el)), std::begin(el));
    if (argmin == 0) {
      return proj0;
    }
    if (argmin == 1) {
      return proj1;
    }
    if (argmin == 2) {
      return proj2;
    }
    return proj3;
  }

  T distanceSquaredTo(Vec3<T> const& p) { return (project(p) - p).normsqr(); }

  T distanceTo(Vec3<T> const& p) { return std::sqrt(distanceSquaredTo(p)); }

  Intersection<Vec3<T>> intersect(Edge3<T> const& e) const {
    Intersection<Vec3<T>> intersection;
    Plane<T> plane(_p1, _p2, _p3);
    T sign1 = plane.sideOf(e.p1());
    T sign2 = plane.sideOf(e.p2());
    if (sign1 == sign2) {
      return intersection;
    }
    Plane<T> plane2(e.p2(), _p1, _p2);
    T sign3 = plane2.sideOf(e.p1());
    Plane<T> plane3(e.p2(), _p2, _p3);
    T sign4 = plane3.sideOf(e.p1());
    Plane<T> plane4(e.p2(), _p3, _p1);
    T sign5 = plane3.sideOf(e.p1());
    if (sign3 != sign4 || sign4 != sign5) {
      return intersection;
    }
    Vec3<T> n = cross(_p2 - _p1, _p3 - _p1);
    T coeff = -e.p1().dot(n - _p1) / e.p1().dot(e.p2() - e.p1());
    intersection.value = e.p1() + (e.p1() - e.p2()) * coeff;
    intersection.exist = true;
    return intersection;
  }

 private:
  Vec3<T> _p1;
  Vec3<T> _p2;
  Vec3<T> _p3;
};

using Trianglef = Triangle<float>;

struct Sphere {
  Vec3f center;
  float squaredRadius;
};

class Box {
 public:
  Box() = default;
  Box(Vec3f center, Vec3f halfLength)
      : center(center), halfLength(halfLength) {}

  Vec3f center;
  Vec3f halfLength;
};

template <typename T, int Dim>
class Shape {
 public:
  using vec_type = Vec<T, Dim>;
  using self_type = Shape<T, Dim>;

 private:
  std::vector<vec_type> _pts;
  vec_type _center;

 public:
  std::vector<vec_type> const& pts() const { return _pts; }
  vec_type const& center() const { return _center; }

  static self_type edge(vec_type const& v1, vec_type const& v2) {
    self_type s;
    s._pts.push_back(v1);
    s._pts.push_back(v2);
    s._center = (v1 + v2) * 0.5f;
    return s;
  }

  static self_type triangle(vec_type const& v1, vec_type const& v2,
                            vec_type const& v3) {
    self_type s;
    s._pts.push_back(v1);
    s._pts.push_back(v2);
    s._pts.push_back(v2);
    s._pts.push_back(v3);
    s._pts.push_back(v3);
    s._pts.push_back(v1);
    return s;
  }

  static self_type box(Box const& b) {
    self_type s;
    Vec3f sign = b.halfLength;
    std::vector<vec_type> v;
    for (int sx = -1; sx < 2; sx += 2) {
      for (int sy = -1; sy < 2; sy += 2) {
        for (int sz = -1; sz < 2; sz += 2) {
          sign.x() = sx * b.halfLength.x();
          sign.y() = sy * b.halfLength.y();
          sign.z() = sz * b.halfLength.z();
          v.push_back(b.center + sign);
        }
      }
    }
    for (std::size_t i = 0; i < v.size(); ++i) {
      for (std::size_t j = i + 1; j < v.size(); ++j) {
        s._pts.push_back(v[i]);
        s._pts.push_back(v[j]);
      }
    }
    return s;
  }
};
using Shape3f = Shape<float, 3>;

}  // namespace arty

#endif  // GEOMETRY_HPP
