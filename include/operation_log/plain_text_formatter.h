#ifndef _OPERATION_LOG_PLAIN_TEXT_FORMATTER_H
#define _OPERATION_LOG_PLAIN_TEXT_FORMATTER_H

#include <functional>
#include <ostream>
#include <string>
#include <vector>

#include "formatter_base.h"
#include "function_info.h"
#include "value_formatter_i.h"


namespace operation_log
{

// A class which receives operation log messages, formats them as plain text,
// and writes them to an `ostream`.
class PlainTextFormatter : public FormatterBase
{
public:
    inline PlainTextFormatter(std::ostream &output_stream)
    : FormatterBase(output_stream)
    {}

protected:
    void write_message_prefix() override
    {
        for (int c = 0; c < 2 * filtered_stack_depth; ++c)
        {
            output.get() << ' ';
        }
    }

    void write_message_value(std::string message) override
    {
        output.get() << message << std::endl;
    }

    void write_dump_vars_suffix() override
    {
        output.get() << std::endl;
    }

    void write_dump_vars_separator() override
    {
        output.get() << "," << std::endl;
    }

    void write_dump_var(std::string name, ValueFormatterI &value_formatter)
    {
        output.get() << name << " = " << value_formatter.to_text();
    }

    virtual void write_function_suffix()
    {
        output.get() << std::endl;
    }

    void write_function_return_type_and_name(
        std::string return_type, std::string name) override
    {
        output.get() << return_type << " " << name;
    }

    void write_function_args_prefix() override
    {
        output.get() << "(";
    }

    void write_function_args_suffix() override
    {
        output.get() << ")";
    }

    void write_function_args_separator() override
    {
        output.get() << ", ";
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name,
        ValueFormatterI &value_formatter)
    {
        output.get() << type_name << " " << parameter_name << " = " << value_formatter.to_text();
    }

    void write_function_extra_info(std::string info) override
    {
        output.get() << " " << info;
    }
};

}

#endif // _OPERATION_LOG_PLAIN_TEXT_FORMATTER_H