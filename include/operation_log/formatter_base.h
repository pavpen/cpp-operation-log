#ifndef _OPERATION_LOG_FORMATTER_BASE_H
#define _OPERATION_LOG_FORMATTER_BASE_H

#include <ostream>
#include <string>
#include <vector>

#include "function_info.h"

namespace operation_log
{

// A base class for receiving operation log messages, formatting them, and
// writing them to an `ostream`.
class FormatterBase
{
public:
    FormatterBase(std::ostream &output_stream)
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
        write_message_value(message);
    }

    template <typename... VarTs>
    void dump_vars(const std::vector<std::string> &names, VarTs... vars)
    {
        var_i = 0;
        this->names = names;
        write_message_prefix();
        write_dump_vars_prefix();
        write_vars(vars...);
        write_dump_vars_suffix();
    }

    template <typename... ArgTs>
    void log_function_entry(FunctionInfo &function_info, ArgTs... args)
    {
        argument_i = 0;
        this->function_info = function_info;
        write_message_prefix();
        write_function_prefix();
        write_function_return_type_and_name(
            function_info.get_return_type(),
            use_function_long_name ?
                function_info.get_full_name() :
                function_info.get_short_name());
        write_function_args_prefix();
        write_function_args(args...);
        write_function_args_suffix();
        if (output_function_extra_info)
        {
            write_function_extra_info(function_info.get_extra_information());
        }
        write_function_suffix();
        ++filtered_stack_depth;
    }

    void enter_function()
    {
        ++stack_depth;
    }

    void log_function_exit(FunctionInfo &function_info)
    {
        write_function_exit(function_info);
        --filtered_stack_depth;
    }

    void exit_function()
    {
        --stack_depth;
    }

protected:
    bool output_function_extra_info = false;
    bool use_function_long_name = false;

    std::reference_wrapper<std::ostream> output;
    int var_i;
    int argument_i;
    int stack_depth = 0;
    int filtered_stack_depth = 0;
    std::vector<std::string> names;
    FunctionInfo function_info;

    virtual void write_message_prefix()
    {}

    virtual void write_message_value(std::string message) = 0;

    virtual void write_dump_vars_prefix()
    {}

    virtual void write_dump_vars_suffix()
    {}

    virtual void write_dump_vars_separator()
    {}

    virtual void write_dump_var(std::string name, bool value) = 0;

    virtual void write_dump_var(std::string name, short value) = 0;

    virtual void write_dump_var(std::string name, unsigned short value) = 0;

    virtual void write_dump_var(std::string name, int value) = 0;

    virtual void write_dump_var(std::string name, unsigned int value) = 0;

    virtual void write_dump_var(std::string name, long value) = 0;

    virtual void write_dump_var(std::string name, unsigned long value) = 0;

    virtual void write_dump_var(std::string name, float value) = 0;

    virtual void write_dump_var(std::string name, double value) = 0;

    virtual void write_dump_var(std::string name, long double value) = 0;

    virtual void write_dump_var(std::string name, void* value) = 0;

    template <typename Var0T>
    void write_vars(Var0T var0)
    {
        if (var_i > 0)
        {
            write_dump_vars_separator();
        }
        write_dump_var(names[var_i], var0);
        ++var_i;
    }

    template <typename Var0T, typename... VarTs>
    void write_vars(Var0T var0, VarTs... vars)
    {
        write_vars(var0);
        write_vars(vars...);
    }

    virtual void write_function_prefix()
    {}

    virtual void write_function_suffix()
    {}

    virtual void write_function_return_type_and_name(
        std::string return_type, std::string name) = 0;

    virtual void write_function_args_prefix() = 0;

    virtual void write_function_args_suffix() = 0;

    virtual void write_function_args_separator()
    {}

    virtual void write_function_arg(
        std::string type_name, std::string parameter_name, bool value) = 0;

    virtual void write_function_arg(
        std::string type_name, std::string parameter_name, short value) = 0;

    virtual void write_function_arg(
        std::string type_name, std::string parameter_name, unsigned short value) = 0;

    virtual void write_function_arg(
        std::string type_name, std::string parameter_name, int value) = 0;

    virtual void write_function_arg(
        std::string type_name, std::string parameter_name, unsigned int value) = 0;

    virtual void write_function_arg(
        std::string type_name, std::string parameter_name, long value) = 0;

    virtual void write_function_arg(
        std::string type_name, std::string parameter_name, unsigned long value) = 0;

    virtual void write_function_arg(
        std::string type_name, std::string parameter_name, float value) = 0;

    virtual void write_function_arg(
        std::string type_name, std::string parameter_name, double value) = 0;

    virtual void write_function_arg(
        std::string type_name, std::string parameter_name, long double value) = 0;

    virtual void write_function_arg(
        std::string type_name, std::string parameter_name, void* value) = 0;

    void write_function_args()
    {}

    template <typename Arg0T>
    void write_function_args(Arg0T arg0)
    {
        if (argument_i > 0)
        {
            write_function_args_separator();
        }
        write_function_arg(
            function_info.get_argument_type(argument_i),
            function_info.get_argument_name(argument_i),
            arg0);
        ++argument_i;
    }

    template <typename Arg0T, typename... ArgTs>
    void write_function_args(Arg0T arg0, ArgTs... args)
    {
        write_function_args(arg0);
        write_function_args(args...);
    }

    virtual void write_function_extra_info(std::string info) = 0;

    virtual void write_function_exit(FunctionInfo &function_info)
    {}
};

}

#endif // _OPERATION_LOG_FORMATTER_BASE_H