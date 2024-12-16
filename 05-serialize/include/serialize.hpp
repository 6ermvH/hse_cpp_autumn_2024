#include <sstream>
#include <string>
#include <cassert>
#include <cstdint>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';

public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }

    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT... args)
    {
        return process(std::forward<ArgsT>(args)...);
    }

private:
    std::ostream& out_;

    Error process(uint64_t arg)
    {
        out_ << arg << Separator;
        return Error::NoError;
    }

    Error process(bool arg)
    {
        out_ << (arg ? "true" : "false") << Separator;
        return Error::NoError;
    }

    template <class T, class... ArgsT>
    Error process(T&& arg, ArgsT&&... args)
    {
        if (process(std::forward<T>(arg)) == Error::NoError)
            return process(std::forward<ArgsT>(args)...);

        return Error::CorruptedArchive;
    }
};

class Deserializer
{
    static constexpr char Separator = ' ';

public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {
    }

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... ArgsT>
    Error operator()(ArgsT&... args)
    {
        return process(args...);
    }

private:
    std::istream& in_;

    Error process(uint64_t& arg)
    {
        std::string text;
        in_ >> text;

        try {
            arg = std::stoull(text);
        } catch (...) {
            return Error::CorruptedArchive;
        }

        return Error::NoError;
    }

    Error process(bool& arg)
    {
        std::string text;
        in_ >> text;

        if (text == "true")
            arg = true;
        else if (text == "false")
            arg = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }

    template <class T, class... ArgsT>
    Error process(T& arg, ArgsT&... args)
    {
        if (process(arg) == Error::NoError)
            return process(args...);

        return Error::CorruptedArchive;
    }
};

