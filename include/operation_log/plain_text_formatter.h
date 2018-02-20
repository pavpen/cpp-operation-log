#ifndef _OPERATION_LOG_PLAIN_TEXT_FORMATTER_H
#define _OPERATION_LOG_PLAIN_TEXT_FORMATTER_H

#include <functional>
#include <ostream>
#include <string>
#include <vector>

#include "function_info.h"

namespace operation_log
{

// A class which receives operation log messages, formats them as plain text,
// and writes them to an `ostream`.
class PlainTextFormatter
{
public:
    inline PlainTextFormatter(std::ostream &output_stream)
    : output(output_stream)
    {}

    std::ostream& get_output_stream()
    {
        return output;
    }

    void set_output_stream(std::ostream &value)
    {
        output = value;
    }

    void write_message(std::string message)
    {
        write_message_prefix();
        output.get() << message << std::endl;
    }

    template <typename... VarTs>
    void dump_vars(const std::vector<std::string> &names, VarTs... vars)
    {
        var_i = 0;
        this->names = names;
        write_message_prefix();
        write_vars(vars...);
        output.get() << std::endl;
    }

    template <typename... ArgTs>
    void log_function_entry(FunctionInfo &function_info, ArgTs... args)
    {
        argument_i = 0;
        this->function_info = function_info;
        write_message_prefix();
        output.get() << function_info.get_return_type() << " ";
        if (use_function_long_name)
        {
            output.get() << function_info.get_full_name();
        }
        else
        {
            output.get() << function_info.get_short_name();
        }
        output.get() << "(";
        write_function_args(args...);
        output.get() << ")";
        if (output_function_extra_info)
        {
            output.get() << " " << function_info.get_extra_information();
        }
        output.get() << std::endl;
    }

    void enter_function()
    {
        ++stack_depth;
    }

    void exit_function()
    {
        --stack_depth;
    }

private:
    bool output_function_extra_info = false;
    bool use_function_long_name = false;

    std::reference_wrapper<std::ostream> output;
    int var_i;
    int argument_i;
    int stack_depth;
    std::vector<std::string> names;
    FunctionInfo function_info;

    void write_message_prefix()
    {
        for (int c = 0; c < 2 * stack_depth; ++c)
        {
            output.get() << ' ';
        }
    }

    template <typename Var0T>
    void write_vars(Var0T var0)
    {
        if (var_i > 0)
        {
            output.get() << "," << std::endl;
        }
        output.get() << names[var_i] << " = " << var0;
        ++var_i;
    }

    template <typename Var0T, typename... VarTs>
    void write_vars(Var0T var0, VarTs... vars)
    {
        if (var_i > 0)
        {
            output.get() << "," << std::endl;
        }
        output.get() << names[var_i] << " = " << var0;
        ++var_i;
        write_vars(vars...);
    }

    void write_function_args()
    {}

    template <typename Arg0T>
    void write_function_args(Arg0T arg0)
    {
        if (argument_i > 0)
        {
            output.get() << ", ";
        }
        output.get() << function_info.get_argument_type(argument_i) << " " <<
            function_info.get_argument_name(argument_i) << "=" << arg0;
        ++argument_i;
    }

    template <typename Arg0T, typename... ArgTs>
    void write_function_args(Arg0T arg0, ArgTs... args)
    {
        if (argument_i > 0)
        {
            output.get() << ", ";
        }
        output.get() << function_info.get_argument_type(argument_i) << " " <<
            function_info.get_argument_name(argument_i) << "=" << arg0;
        ++argument_i;
        write_function_args(args...);
    }
};

}

#endif // _OPERATION_LOG_PLAIN_TEXT_FORMATTER_H