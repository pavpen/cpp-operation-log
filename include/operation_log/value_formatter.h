#ifndef _OPERATION_LOG_VALUE_FORMATTER_H
#define _OPERATION_LOG_VALUE_FORMATTER_H

#include "value_formatter_base.h"


namespace operation_log
{

// The public class for formatting variable values.
//
// Using an intermediary base class allows for user-defined overrides of base
// class specializations for already defined types.
//
// E.g., the following specialization is already defined:
//
// template<>
// ValueFormatterBase<std::string> {};
//
// The user can override it by defining:
//
// template <>
// class ValueFormatter<std::string> : public ValueFormatterI
// {
//     private:
//     const std::string value;
//
//     public:
//
//     ValueFormatter(const std::string value)
//     : value(value)
//     {}
//
//     std::string to_text() override
//     {
//          // Custom implementation.
//     }
//
//     std::string to_html() override
//     {
//          // Custom implementation.
//     }
// };

template <typename T>
class ValueFormatter : public ValueFormatterBase<T>
{
    public:

    using ValueFormatterBase<T>::ValueFormatterBase;
};

}

#endif // _OPERATION_LOG_VALUE_FORMATTER_H
