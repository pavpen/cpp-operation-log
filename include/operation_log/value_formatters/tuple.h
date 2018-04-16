#ifndef _OPERATION_LOG_VALUE_FORMATTERS_TUPLE_H
#define _OPERATION_LOG_VALUE_FORMATTERS_TUPLE_H

#include <tuple>

#include "../value_formatter_base.h"
#include "../value_formatter_i.h"


namespace operation_log
{
template <typename T>
class ValueFormatter;

namespace helpers
{

std::string format_to_text(ValueFormatterI &formatter)
{
    return formatter.to_text();
}

std::string format_to_html(ValueFormatterI &formatter)
{
    return formatter.to_html();
}

}

// A default value formatter for the `std::tuple` data type.
template <typename... Ts>
class ValueFormatterBase<std::tuple<Ts...>> : public ValueFormatterI
{
    private:

    const std::tuple<Ts...> value;

    public:

    // Receives the value to format.
    ValueFormatterBase(const std::tuple<Ts...> value)
    : value(value)
    {}

    // Formats values for plain text logs.
    std::string to_text() override
    {
        std::stringstream res;

        res << "std::tuple( ";
        write_elements<0, helpers::format_to_text>(res);
        res << " )";

        return res.str();
    }

    // Formats values for HTML logs.
    std::string to_html() override
    {
        std::stringstream res;

        res << "std::tuple( ";
        write_elements<0, helpers::format_to_html>(res);
        res << " )";

        return res.str();
    }

    private:

    template <std::size_t VarI, std::string FormatFunctor(ValueFormatterI&)>
    inline typename std::enable_if<VarI == sizeof...(Ts), void>::type
    write_elements(std::stringstream &res)
    {}

    template <std::size_t VarI, std::string FormatFunctor(ValueFormatterI&)>
    inline typename std::enable_if<VarI < sizeof...(Ts), void>::type
    write_elements(std::stringstream &res)
    {
        if (VarI > 0)
        {
            res << ", ";
        }

        ValueFormatter<typename std::tuple_element<VarI, std::tuple<Ts...>>::type>
            value_formatter(std::get<VarI>(value));

        res << FormatFunctor(value_formatter);

        write_elements<VarI + 1, FormatFunctor>(res);
    }
};

}

#endif // _OPERATION_LOG_VALUE_FORMATTERS_TUPLE_H
