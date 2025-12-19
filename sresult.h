#include <iostream>
#include <type_traits>
#include <utility>
#include <variant>
namespace sresult {
template <typename E>
struct unexpected {
  E value;
  unexpected(const E& value) : value(value) {}
  unexpected(E&& value) : value(std::move(value)) {}
};
template <typename T, typename E>
class expected {
  std::variant<T, unexpected<E>> val;
  bool has_error() const {
    return std::holds_alternative<unexpected<E>>(this->val);
  }
  template <typename Self, typename Function>
  static constexpr auto and_then_impl(Self&& self, Function fn)
      -> decltype(fn(T())) {
    if (self.has_value()) return fn(std::forward<Self>(self).value());
    return unexpected(std::forward<Self>(self).error());
  }
  template <typename Self, typename Function>
  static constexpr auto transform_impl(Self&& self, Function fn)
      -> expected<decltype(fn(T())), E> {
    if (self.has_value()) return fn(std::forward<Self>(self).value());
    return unexpected(std::forward<Self>(self).error());
  }
  template <typename Self, typename Function>
  static constexpr auto transform_error_impl(Self&& self, Function fn)
      -> expected<T, decltype(fn(E()))> {
    if (self.has_error())
      return unexpected(fn(std::forward<Self>(self).error()));
    return std::forward<Self>(self).value();
  }

 public:
  constexpr expected() {}
  constexpr expected(const T& arg_val) : val(arg_val) {}
  constexpr expected(T&& arg_val) : val(std::move(arg_val)) {}
  constexpr expected(const unexpected<E>& arg_val) : val(arg_val) {}
  constexpr expected(unexpected<E>&& arg_val) : val(std::move(arg_val)) {}
  operator bool() const { return std::holds_alternative<T>(this->val); }
  bool has_value() const { return this->operator bool(); }
  constexpr T&& value() && { return std::move(std::get<0>(this->val)); }
  constexpr const T&& value() const&& { return std::get<0>(this->val); }
  constexpr T& value() & { return std::get<0>(this->val); }
  constexpr const T& value() const& { return std::get<0>(this->val); }
  constexpr E&& error() && { return std::move(std::get<1>(this->val).value); }
  constexpr const E&& error() const&& { return std::get<1>(this->val).value; }
  constexpr E& error() & { return std::get<1>(this->val).value; }
  constexpr const E& error() const& { return std::get<1>(this->val).value; }
  template <typename Function>
  constexpr auto and_then(Function fn) & {
    return and_then_impl(*this, fn);
  }
  template <typename Function>
  constexpr auto and_then(Function fn) && {
    return and_then_impl(std::move(*this), fn);
  }
  template <typename Function>
  constexpr auto and_then(Function fn) const& {
    return and_then_impl(*this, fn);
  }
  template <typename Function>
  constexpr auto and_then(Function fn) const&& {
    return and_then_impl(std::move(*this), fn);
  }
  template <typename Function>
  constexpr auto transform(Function fn) & {
    return transform_impl(*this, fn);
  }
  template <typename Function>
  constexpr auto transform(Function fn) && {
    return transform_impl(std::move(*this), fn);
  }
  template <typename Function>
  constexpr auto transform(Function fn) const& {
    return transform_impl(*this, fn);
  }
  template <typename Function>
  constexpr auto transform(Function fn) const&& {
    return transform_impl(std::move(*this), fn);
  }
  template <typename Function>
  constexpr auto transform_error(Function fn) & {
    return transform_error_impl(*this, fn);
  }
  template <typename Function>
  constexpr auto transform_error(Function fn) && {
    return transform_error_impl(std::move(*this), fn);
  }
  template <typename Function>
  constexpr auto transform_error(Function fn) const& {
    return transform_error_impl(*this, fn);
  }
  template <typename Function>
  constexpr auto transform_error(Function fn) const&& {
    return transform_error_impl(std::move(*this), fn);
  }
// 文字列リテラルをマルチバイトとワイドで切り替える一時的マクロ
#define TEXT_FOR(CharT, str)                      \
  []() {                                          \
    if constexpr (std::is_same_v<CharT, wchar_t>) \
      return L##str;                              \
    else                                          \
      return str;                                 \
  }()
  template <typename TChar>
  friend std::basic_ostream<TChar>& operator<<(std::basic_ostream<TChar>& os,
                                               const expected& r) {
    if (r.has_value())
      os << TEXT_FOR(TChar, "Ok(") << r.value() << TEXT_FOR(TChar, ")");
    else
      os << TEXT_FOR(TChar, "Err(") << r.error() << TEXT_FOR(TChar, ")");
    return os;
  }
// マクロ解除
#undef TEXT_FOR
};
}  // namespace sresult