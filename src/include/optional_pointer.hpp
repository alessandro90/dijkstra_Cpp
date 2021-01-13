#ifndef OPTIONAL_POINTER_HPP
#define OPTIONAL_POINTER_HPP

#include <optional>

template <typename T>
class OptionalPointer {
public:
    OptionalPointer(T* ptr)
        : mPtr { ptr }
    {
    }

    OptionalPointer(std::nullopt_t)
        : mPtr { std::nullopt }
    {
    }

    OptionalPointer(std::nullptr_t) = delete;

    [[nodiscard]] bool has_value() const { return mPtr.has_value(); }
    [[nodiscard]] operator bool() const { return has_value(); }

    [[nodiscard]] T* operator->() { return *mPtr; }
    [[nodiscard]] T const* operator->() const { return *mPtr; }

    [[nodiscard]] T& operator*() { return *(*mPtr); }
    [[nodiscard]] T const& operator*() const { return *(*mPtr); }

private:
    std::optional<T*> mPtr;
};

#endif