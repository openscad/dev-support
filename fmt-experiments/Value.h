#include <initializer_list>
#include <string_view>
#include <variant>
#include <vector>
#include <cmath>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ranges.h>

struct Value {
  struct StringType {
    StringType() = default;
    StringType(std::string s) : str{std::move(s)} { }
    std::string str;
  };

  struct VectorType {
    VectorType() = default;
    VectorType(std::initializer_list<Value> v) : vec{v} {}
    std::vector<Value> vec;
  };

  struct Numeric {
    Numeric() = default;
    Numeric(double d) : value(d) { }
    double value = 0.0;
  };

  using Variant = std::variant<std::monostate, Numeric, VectorType, StringType>;

  Variant value;
};

using VectorType = Value::VectorType;
using StringType = Value::StringType;
using Numeric = Value::Numeric;


template <typename FormatContext>
struct formatter_visitor {
  FormatContext& ctx;
  std::string_view fmt_sv;

  formatter_visitor(FormatContext& ctx, std::string_view fmt_sv) : ctx(ctx), fmt_sv(fmt_sv) { }

  template <typename T>
  auto operator()(const T& op) -> decltype(ctx.out()) {
    return fmt::format_to(ctx.out(), fmt_sv, op);
  }

  auto operator()(const std::monostate& /* op */) -> decltype(ctx.out()) {
    return fmt::format_to(ctx.out(), "undef");
    //return ctx.out();
  }
};

template <> struct fmt::formatter<Value> {
  // format specifiers (in between the curly braces `{}`) shouldn't be more than 4 or 5 characters
  static constexpr size_t buflen = 32;
  char fmt_str[buflen];
  size_t length = 0;

  std::string_view fmt_sv() const { return {&fmt_str[0], length}; }

  // parse up to the closing brace or end of parse context (or until buflen reached)
  // fmt_str must be constructed in a char[] buffer to remain constexpr under C++17
  constexpr auto parse(fmt::format_parse_context& ctx) -> decltype(ctx.begin()) {
    auto it = ctx.begin(), end = ctx.end();
    fmt_str[length++] = '{';
    fmt_str[length++] = ':';
    while (it != end && *it != '}' && length < buflen) {
      fmt_str[length++] = *(it++);
    }
    fmt_str[length++] = '}';
    return it;
  }

  template <typename FormatContext>
  auto format(const Value& v, FormatContext& ctx) const -> decltype(ctx.out()) {
    // using fmt's builtin std::variant handling automatically wraps the output in "variant({})",
    // with seemingly no way to customize
    //return fmt::format_to(ctx.out(), this->fmt_sv(), v.value);

    // So we use our own visitor
    return std::visit(formatter_visitor(ctx, this->fmt_sv()), v.value);
  }
};


// Numeric, testing two different implementations
#if 0 // use inherited parse and fmt_sv from formatter<Value>
template <> struct fmt::formatter<Numeric> : fmt::formatter<Value> {
  template <typename FormatContext>
  auto format(const Numeric& n, FormatContext& ctx) const -> decltype(ctx.out()) {
    return std::isnan(n.value) ?
      // avoid output of "-nan"
      fmt::format_to(ctx.out(), "nan") :
      // otherwise pass fmt_sv along to builtin fmt::formatter<double>
      fmt::format_to(ctx.out(), this->fmt_sv(), n.value);
  }
};
#else // otherwise use parse inherited from builtin formatter<double>
template <> struct fmt::formatter<Numeric> : fmt::formatter<double> {

  // parse function inherited from formatter<double>
  template <typename FormatContext>
  auto format(const Numeric& n, FormatContext& ctx) const -> decltype(ctx.out()) {
    return std::isnan(n.value) ?
      // avoid output of "-nan"
      fmt::format_to(ctx.out(), "nan") :
      // otherwise use inherited format function
      formatter<double>::format(n.value, ctx);
  }
};
#endif // Numeric alternate defintions





// VectorType, testing two different implementations
#if 1 // Use parse and fmt_sv function inherited from formatter<Value>

template <> struct fmt::formatter<VectorType> : fmt::formatter<Value> {

  template <typename FormatContext>
  auto format(const VectorType& v, FormatContext& ctx) const -> decltype(ctx.out()) {
    // use ranges format spec (double colon), while inserting previously parsed spec
    //   One major pain point is that in order to put literal brace characters `{` and `}`
    //   in format output, documentation says to double them: "{{" and "}}",
    //   but this seems to always break parsing of the format.
    //return fmt::format_to(ctx.out(), fmt::format("{{::{}}}", this->fmt_sv()), v.vec);

    // "dynamic formatting" doesn't seem to work here
    //return fmt::format_to(ctx.out(), "{::{}}", v.vec, this->fmt_sv());

    // only method that seems to work so far:
    return fmt::format_to(ctx.out(), fmt::format("[{}]", this->fmt_sv()), fmt::join(v.vec, ", "));
  }
};

#else // Otherwise use custom parser (more runtime errors)

template <> struct fmt::formatter<VectorType> {

  // Attempt to build the range specifier into the format buffer,
  // intended to be used with format function body:
  //   return fmt::format_to(ctx.out(), this->fmt_sv(), v.vec);
  // Still only got format errors.

  // format specifiers (in between the curly braces `{}`) shouldn't be more than 4 or 5 characters
  static constexpr size_t buflen = 32;
  char fmt_str[buflen];
  size_t length = 0;

  std::string_view fmt_sv() const { return {&fmt_str[0], length}; }

  // parse up to the closing brace or end of parse context (or until buflen reached)
  // fmt_str must be constructed in a char[] buffer to remain constexpr under C++17
  constexpr auto parse(fmt::format_parse_context& ctx) -> decltype(ctx.begin()) {
    auto it = ctx.begin(), end = ctx.end();
    fmt_str[length++] = '{';
    fmt_str[length++] = ':';
    fmt_str[length++] = ':';
    fmt_str[length++] = '{';
    fmt_str[length++] = ':';
    while (it != end && *it != '}' && length < buflen) {
      fmt_str[length++] = *(it++);
    }
    fmt_str[length++] = '}';
    fmt_str[length++] = '}';
    return it;
  }

  template <typename FormatContext>
  auto format(const VectorType& v, FormatContext& ctx) const -> decltype(ctx.out()) {
    return fmt::format_to(ctx.out(), this->fmt_sv(), v.vec);
  };

};

#endif // VectorType alternate defintions


// StringType
template <> struct fmt::formatter<StringType> : fmt::formatter<Value> {
  // use formatter<Value>'s parse function to consume the parse context characters,
  // but ignore `fmt_sv()` and only use static quoted string formatting:
  template <typename FormatContext>
  auto format(const StringType& v, FormatContext& ctx) const -> decltype(ctx.out()) {
    return fmt::format_to(ctx.out(), "\"{}\"", v.str);
  }
};
