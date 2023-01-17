#include <iostream>
#include <fmt/format.h>
#include <fmt/compile.h>

#include "Value.h"

int main() {
  Value x,y,z;
  double nan = 0.0/0.0;
  Numeric n1(0.321 / 0.123);
  Numeric n2(1.0 / 0.17);
  Numeric inf(1.0 / 0.0);
  Numeric ninf(-1.0 / 0.0);
  StringType hi{"Hello World!"};

  x.value = n1;
  y.value = VectorType{ Value{n1}, Value{inf}, Value{ninf}, Value{nan}, Value{-nan}};
  z.value = VectorType{ Value{n1}, Value{ hi }, Value{ VectorType{ Value{n2}, Value{-nan} } } }; // more mixed types including StringType now

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
  // Problem:
  //   Seems inefficient to parse and re-build the format specifier at every level of nesting
  //   (see function: `formatter<Value>::parse(...)`, inherited by Variant's held types in some cases)

  // echo() default precision is `{:.6g}`  the 'g' is implied / default.
  fmt::print("{:.6}\n", n1.value); // Try directly on double
  fmt::print("{:.6}\n", n1);       // Try on Numeric
  fmt::print("{:.6}\n", x);        // Try on Value of Numeric
  fmt::print("{:.6}\n", y);        // Try on Value of VectorType of Numerics
  fmt::print("{:.6}\n", z);        // Try on Value of VectorType with nested StringType and VectorType

  // "Dynamic precision" workaround
  int prec = 9;
  fmt::print(fmt::format("{{:.{}g}}\n", prec), x); // Not sure how to avoid runtime error here

  // https://fmt.dev/latest/syntax.html#format-examples  scroll down to "Dynamic precision:"
  fmt::print("{:.{}g}\n", x, prec); // Not sure how to avoid runtime error here

  return 0;
}