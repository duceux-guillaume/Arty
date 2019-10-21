#ifndef MATH_H
#define MATH_H

namespace arty {

class Num {
 private:
  bool _neg;
  int _num;
  int _den;
  int _prec;

 public:
};

template <typename T, int Rows, int Cols>
class Mat {
 private:
  T _data[Rows * Cols];

 public:
};

}  // namespace arty

#endif  // MATH_H
