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
    explicit File(std::string_view fname, openmode mode = in)
        : stream { fname.data(), mode }
    {
        if (!stateok())
            throw FileException { state() };
    }

    void write(std::string_view text)
    {
        stream.write(text.data(), static_cast<std::streamsize>(text.size()));
    }

    void append(std::string_view text) { stream << text.data(); }

    std::string read()
    {
        return std::string {
            std::istreambuf_iterator<char>(stream),
            std::istreambuf_iterator<char>()
        };
    }

    std::string readline()
    {
        std::string str;
        std::getline(stream, str);
        return str;
    }

    std::ios_base::iostate state() const { return stream.rdstate(); }

    bool stateok() const { return state() == std::ios_base::goodbit; }

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
        explicit iterator(std::fstream* s_)
            : s { s_ }
        {
            if (s != nullptr && !std::getline(*s, str))
                setnull();
        }

        bool operator==(iterator const& other) const { return s == other.s; }

        bool operator!=(iterator const& other) const { return !(*this == other); }

        value_type operator*() const { return str; }

        pointer operator->() const { return &str; }

        iterator& operator++()
        {
            if (!std::getline(*s, str))
                setnull();
            return *this;
        }

        iterator operator++(int)
        {
            iterator it = *this;
            if (!std::getline(*s, str))
                setnull();
            return it;
        }

    private:
        void setnull() { s = nullptr; }
        std::fstream* s { nullptr };
        std::string str {};
    };

    auto begin() { return iterator { &stream }; }

    auto end() { return iterator {}; }

private:
    std::fstream stream;
};
}

#endif
