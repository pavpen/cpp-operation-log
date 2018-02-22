#ifndef _OPERATION_LOG_PLAIN_TEXT_FORMATTER_H
#define _OPERATION_LOG_PLAIN_TEXT_FORMATTER_H

#include <functional>
#include <ostream>
#include <string>
#include <vector>

#include "formatter_base.h"
#include "function_info.h"

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
        for (int c = 0; c < 2 * stack_depth; ++c)
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

    void write_dump_var(std::string name, bool value)
    {
        output.get() << name << " = " << value;
    }

    void write_dump_var(std::string name, short value)
    {
        output.get() << name << " = " << value;
    }

    void write_dump_var(std::string name, unsigned short value)
    {
        output.get() << name << " = " << value;
    }

    void write_dump_var(std::string name, int value)
    {
        output.get() << name << " = " << value;
    }

    void write_dump_var(std::string name, unsigned int value)
    {
        output.get() << name << " = " << value;
    }

    void write_dump_var(std::string name, long value)
    {
        output.get() << name << " = " << value;
    }

    void write_dump_var(std::string name, unsigned long value)
    {
        output.get() << name << " = " << value;
    }

    void write_dump_var(std::string name, float value)
    {
        output.get() << name << " = " << value;
    }

    void write_dump_var(std::string name, double value)
    {
        output.get() << name << " = " << value;
    }

    void write_dump_var(std::string name, long double value)
    {
        output.get() << name << " = " << value;
    }

    void write_dump_var(std::string name, void* value)
    {
        output.get() << name << " = " << value;
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
        std::string type_name, std::string parameter_name, bool value)
    {
        output.get() << type_name << " " << parameter_name << " = " << value;
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, short value)
    {
        output.get() << type_name << " " << parameter_name << " = " << value;
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, unsigned short value)
    {
        output.get() << type_name << " " << parameter_name << " = " << value;
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, int value)
    {
        output.get() << type_name << " " << parameter_name << " = " << value;
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, unsigned int value)
    {
        output.get() << type_name << " " << parameter_name << " = " << value;
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, long value)
    {
        output.get() << type_name << " " << parameter_name << " = " << value;
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, unsigned long value)
    {
        output.get() << type_name << " " << parameter_name << " = " << value;
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, float value)
    {
        output.get() << type_name << " " << parameter_name << " = " << value;
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, double value)
    {
        output.get() << type_name << " " << parameter_name << " = " << value;
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, long double value)
    {
        output.get() << type_name << " " << parameter_name << " = " << value;
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, void* value)
    {
        output.get() << type_name << " " << parameter_name << " = " << value;
    }

    void write_function_extra_info(std::string info) override
    {
        output.get() << " " << info;
    }
};

}

#endif // _OPERATION_LOG_PLAIN_TEXT_FORMATTER_H