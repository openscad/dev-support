#include <iostream>
#include <fmt/format.h>
#include <fmt/compile.h>

#include "Value.h"

int main() {
  Value x,y;
  double nan = 0.0/0.0;
  Numeric n1(0.321 / 0.123);
  Numeric n2(1.0 / 0.17);
  Numeric inf(1.0 / 0.0);
  Numeric ninf(-1.0 / 0.0);
  
  x.value = n1;
  y.value = VectorType{ Value{n2}, Value{inf}, Value{ninf}, Value{nan}, Value{-nan}};

  // show that double nan with sign bit negative prints the sign
  fmt::print("{} {}\n", nan, -nan); 
  // Numeric wrapper exists just to fix that
  fmt::print("{} {}\n", Numeric(nan), Numeric(-nan)); 
  
  // default to full precision (useful for CSG dumps, used in cache keys)
  fmt::print("x={} y={}\n", x, y);

  // Goal: Pass a formatting spec for double to a Value type, and have that propagate down recursively as needed.
  //       For example when Value is a VectorType of VectorType of Numerics.
  //       This is desirable so that CSG dumps and `echo()` output can be displayed with different precisions.
  //
  // Problem: It is not clear how to pass the formatting data along in a way that libfmt does not produce a runtime error
  
  fmt::print("{:.6}\n", x); // Try to use echo() default precision  (runtime error)
  fmt::print("{:.6}\n", y);

  // dynamic precision ???
  //fmt::print("{:.{}g}\n", x, 4);

  return 0;
}