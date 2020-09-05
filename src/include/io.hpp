#ifndef IO_HPP
#define IO_HPP

#include <exception>
#include <fstream>
#include <iterator>
#include <map>
#include <string>
#include <string_view>
#include <utility>

namespace io {

using openmode = std::ios_base::openmode;

inline static constexpr openmode in = std::ios_base::in;
inline static constexpr openmode out = std::ios_base::out;
inline static constexpr openmode app = std::ios_base::app;
inline static constexpr openmode bin = std::ios_base::binary;

class FileException : std::exception {
public:
    explicit FileException(std::ios_base::iostate state_)
        : state { state_ }
    {
    }
    const char* what() const noexcept override
    {
        return states.at(state).data();
    }

private:
    const std::map<std::ios_base::iostate, std::string_view> states {
        { std::ios_base::goodbit, "goodbit" },
        { std::ios_base::badbit, "badbit" },
        { std::ios_base::failbit, "failbit" },
        { std::ios_base::eofbit, "eofbit" }
    };
    const std::ios_base::iostate state;
};

class File {
public:
    explicit File(std::string_view fname, openmode mode = in);

    void write(std::string_view text);

    void append(std::string_view text);

    [[nodiscard]] std::string read();

    std::string readline();

    [[nodiscard]] std::ios_base::iostate state() const;

    [[nodiscard]] bool stateok() const;

    class iterator {
    public:
        using value_type = std::string_view;
        // NOTE: Does this even makes sense?
        using difference_type = std::ptrdiff_t;
        using pointer = std::string const*;
        using reference = std::string&;
        using iterator_category = std::input_iterator_tag;
        using iterator_concept = std::input_iterator_tag;

        iterator() = default;
        explicit iterator(std::fstream* s_);

        [[nodiscard]] bool operator==(iterator const& other) const;

        [[nodiscard]] bool operator!=(iterator const& other) const;

        [[nodiscard]] value_type operator*() const;

        [[nodiscard]] pointer operator->() const;

        iterator& operator++();

        iterator operator++(int);

    private:
        void setnull();
        std::fstream* s { nullptr };
        std::string str {};
    };

    [[nodiscard]] iterator begin();

    [[nodiscard]] iterator end();

private:
    std::fstream stream;
};
}

#endif
