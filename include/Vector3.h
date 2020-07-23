#ifndef Vector3_H
#define Vector3_H
#include <math.h>

#include <iostream>

template <typename T>
class Vector3 {
 public:
  Vector3() : x(0), y(0), z(0) {}
  Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
  Vector3(T value) : x(value), y(value), z(value) {}
  Vector3(const Vector3<T>& v) : x(v.x), y(v.y), z(v.z) {}
  void print(std::ostream& out) const {
    out << "[" << x << "," << y << "," << z << "]";
  }
  virtual ~Vector3() {}

 public:
  inline T normalize() {
    T L = this->length();
    *this /= L;
    return L;
  }

  inline Vector3<T> getNormalized() const {
    Vector3<T> temp(*this);
    temp /= temp.length();
    return temp;
  }

  inline T length() const { return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2)); }

  inline T dot(const Vector3<T>& v) const {
    return x * v.x + y * v.y + z * v.z;
  }

  inline Vector3<T> cross(const Vector3<T>& v) const {
    Vector3<T> temp;
    temp.x = y * v.z - (v.y * z);
    temp.y = -(x * v.z - (v.x * z));
    temp.z = x * v.y - (v.x * y);
    return temp;
  }

 public:
  inline Vector3<T>& operator=(const Vector3<T>& v) {
    x = v.x;
    y = v.y;
    z = v.z;
    return *this;
  }

  inline Vector3<T>& operator+=(const Vector3<T>& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    return *this;
  }

  inline Vector3<T>& operator-=(const Vector3<T>& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    return *this;
  }

  inline Vector3<T>& operator/=(T value) {
    x /= value;
    y /= value;
    z /= value;
    return *this;
  }

  inline Vector3<T>& operator*=(T value) {
    x *= value;
    y *= value;
    z *= value;
    return *this;
  }

  inline T& operator[](int index) {
    if (index == 0) return x;
    if (index == 1) return y;
    if (index == 2) return z;
    throw;
  }

  inline T operator[](int index) const {
    if (index == 0) return x;
    if (index == 1) return y;
    if (index == 2) return z;
    throw;
  }

  inline Vector3<T> operator-() {
    Vector3<T> temp(*this);
    temp *= -1;
    return temp;
  }

  template <typename U>
  friend std::ostream& operator<<(std::ostream& out, const Vector3<U>& v);

 private:
  T x, y, z;
};

template <typename T>
inline Vector3<T> operator+(const Vector3<T>& a, const Vector3<T>& b) {
  Vector3<T> temp(a);
  temp += b;
  return temp;
}

template <typename T>
inline Vector3<T> operator-(const Vector3<T>& a, const Vector3<T>& b) {
  Vector3<T> temp(a);
  temp -= b;
  return temp;
}

template <typename T>
inline Vector3<T> operator*(const Vector3<T>& a, T value) {
  Vector3<T> temp(a);
  temp *= value;
  return temp;
}

template <typename T>
inline Vector3<T> operator*(T value, const Vector3<T>& a) {
  Vector3<T> temp(a);
  temp *= value;
  return temp;
}

template <typename T>
inline Vector3<T> operator/(const Vector3<T>& a, T value) {
  Vector3<T> temp(a);
  temp /= value;
  return temp;
}

template <typename T>
std::ostream& operator<<(std::ostream& out, const Vector3<T>& v) {
  v.print(out);
  return out;
}
#endif