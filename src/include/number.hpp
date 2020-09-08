#ifndef NUMBER_H
#define NUMBER_H

#include <concepts>
#include <ostream>

#define UNARY(op)                        \
    Number& operator op(Number const& a) \
    {                                    \
        value_ op a.value_;              \
        return *this;                    \
    }

#define UNARY_N(op)          \
    Number& operator op(T a) \
    {                        \
        value_ op a;         \
        return *this;        \
    }

#define TYPE Number<T, TAG>

#define BINARY(op)                                                                             \
    template <typename T, typename Tag>                                                        \
    [[nodiscard]] constexpr auto operator op(Number<T, Tag> const& a, Number<T, Tag> const& b) \
    {                                                                                          \
        return Number<T, Tag> { a.value() op b.value() };                                      \
    }

#define BINARY_N(op)                                                       \
    template <typename T, typename Tag>                                    \
    [[nodiscard]] constexpr auto operator op(Number<T, Tag> const& a, T b) \
    {                                                                      \
        return Number<T, Tag> { a.value() op b };                          \
    }

#define BINARY_NR(op)                                                      \
    template <typename T, typename Tag>                                    \
    [[nodiscard]] constexpr auto operator op(T b, Number<T, Tag> const& a) \
    {                                                                      \
        return Number<T, Tag> { b op a.value() };                          \
    }

namespace num {

template <typename T, typename Tag>
    requires std::integral<T> || std::floating_point<T> struct Number {
public:
    using value_type = T;
    constexpr Number() = default;
    explicit constexpr Number(T c)
        : value_ { c }
    {
    }
    [[nodiscard]] constexpr T value() const { return value_; }

    UNARY(+=)
    UNARY(-=)
    UNARY(*=)
    UNARY(/=)
    UNARY(%=)

    UNARY_N(+=)
    UNARY_N(-=)
    UNARY_N(*=)
    UNARY_N(/=)
    UNARY_N(%=)

    [[nodiscard]] constexpr auto operator<=>(Number const& a) const = default;
    [[nodiscard]] constexpr auto operator<=>(T b) const { return value() <=> b; }

private:
    T value_ {};
};

template <typename T, typename Tag>
[[nodiscard]] constexpr auto operator-(Number<T, Tag> const& a)
{
    return Number<T, Tag> { -a.value() };
}

BINARY(+)
BINARY(-)
BINARY(*)
BINARY(/)
BINARY(%)

BINARY_N(+)
BINARY_N(-)
BINARY_N(*)
BINARY_N(/)
BINARY_N(%)

BINARY_NR(+)
BINARY_NR(-)
BINARY_NR(*)
BINARY_NR(/)
BINARY_NR(%)

template <typename T, typename Tag>
auto& operator<<(std::ostream& os, Number<T, Tag> const& a)
{
    return os << a.value();
}

}

#undef UNARY
#undef UNARY_N
#undef BINARY
#undef BINARY_N
#undef BINARY_NR

#endif
