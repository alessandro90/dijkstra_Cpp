#include "io.hpp"
#include <ios>
#include <iterator>
#include <string_view>

namespace io {

File::File(std::string_view fname, openmode mode)
    : stream { fname.data(), mode }
{
    if (!stateok())
        throw FileException { state() };
}

void File::write(std::string_view text)
{
    stream.write(text.data(), static_cast<std::streamsize>(text.size()));
}

void File::append(std::string_view text) { stream << text.data(); }

std::string File::read()
{
    return std::string {
        std::istreambuf_iterator<char>(stream),
        std::istreambuf_iterator<char>()
    };
}

std::string File::readline()
{
    std::string str;
    std::getline(stream, str);
    return str;
}

std::ios_base::iostate File::state() const { return stream.rdstate(); }

bool File::stateok() const { return state() == std::ios_base::goodbit; }

File::iterator File::begin() { return File::iterator { &stream }; }

File::iterator File::end() { return File::iterator {}; }

File::iterator::iterator(std::fstream* s_)
    : s { s_ }
{
    if (s != nullptr && !std::getline(*s, str))
        setnull();
}

bool File::iterator::operator==(File::iterator const& other) const { return s == other.s; }

bool File::iterator::operator!=(File::iterator const& other) const { return !(*this == other); }

File::iterator::value_type File::iterator::operator*() const { return str; }

File::iterator::pointer File::iterator::operator->() const { return &str; }

File::iterator& File::iterator::operator++()
{
    if (!std::getline(*s, str))
        setnull();
    return *this;
}

File::iterator File::iterator::operator++(int)
{
    File::iterator it = *this;
    ++(*this);
    return it;
}

void File::iterator::setnull() { s = nullptr; }

}