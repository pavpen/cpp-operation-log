#ifndef _OPERATION_LOG_FUNCTION_INFO_H
#define _OPERATION_LOG_FUNCTION_INFO_H

#include <string>
#include <vector>

#include "cpp_parsing.h"

namespace operation_log
{

// A class that describes the properties of a C++ function used in operation
// logging.
class FunctionInfo
{
private:
    std::string return_type;
    std::string full_name;
    std::string short_name;
    std::vector<std::string> argument_types;
    std::vector<std::string> argument_names;
    std::string extra_information;

public:
    inline FunctionInfo()
    {}

    inline FunctionInfo(std::string pretty_function, std::string stringified_arg_list)
    {
        parse_pretty_function(pretty_function);
        argument_names = CppParsing::parse_stringified_list(stringified_arg_list);
    }

    void parse_pretty_function(const std::string &pretty_function);

    inline std::string get_return_type() const
    {
        return return_type;
    }

    inline std::string get_full_name() const
    {
        return full_name;
    }

    inline std::string get_short_name() const
    {
        return short_name;
    }

    inline const std::vector<std::string> get_argument_types() const
    {
        return argument_types;
    }

    inline std::string get_argument_type(int argument_i) const
    {
        return argument_types[argument_i];
    }

    inline const std::vector<std::string> get_argument_names() const
    {
        return argument_names;
    }

    std::string get_argument_name(int argument_i) const
    {
        return argument_names[argument_i];
    }

    inline std::string get_extra_information() const
    {
        return extra_information;
    }

private:
    std::string arg_list_get_type_string(const std::string &s, size_t pos = 0);
};

void FunctionInfo::parse_pretty_function(const std::string &pretty_function)
{
    std::size_t name_end = pretty_function.find("(") - 1;
    std::size_t name_begin = pretty_function.substr(0, name_end).rfind(" ") + 1;

    return_type = pretty_function.substr(0, name_begin - 1);
    full_name = pretty_function.substr(name_begin, name_end - name_begin + 1);

    std::size_t short_name_begin = full_name.rfind("::");

    if (short_name_begin >= full_name.length())
    {
        short_name_begin = 0;
    }
    else
    {
        short_name_begin += 2;
    }

    short_name = full_name.substr(short_name_begin);

    std::size_t arg_list_begin = name_end + 2;
    std::size_t arg_list_end = pretty_function.rfind(")") - 1;

    for (std::size_t i = arg_list_begin; i < arg_list_end; )
    {
        if (pretty_function[i] == ',')
        {
            ++i;
        }
        while (i < arg_list_end && isspace(pretty_function[i]))
        {
            ++i;
        }
        if (i >= arg_list_end)
        {
            break;
        }

        std::string arg_type = arg_list_get_type_string(pretty_function, i);

        argument_types.emplace_back(arg_type);
        i += arg_type.length();
    }

    std::size_t extra_information_begin = arg_list_end + 2;
    std::size_t len = pretty_function.length();
    while (extra_information_begin < len && isspace(pretty_function[extra_information_begin]))
    {
        ++extra_information_begin;
    }
    extra_information = pretty_function.substr(extra_information_begin);
}

std::string FunctionInfo::arg_list_get_type_string(const std::string &s, size_t pos)
{
    std::size_t len = s.length();
    std::size_t i;

    // Count brackets and braces to parse a type string:
    int bracket_count = 0;
    int angle_count = 0;
    int square_count = 0;
    int brace_count = 0;

    for (i = pos; i < len; ++i)
    {
        if (bracket_count == 0 && angle_count == 0 && square_count == 0
        && brace_count == 0 && s[i] == ',')
        {
            return s.substr(pos, i - pos);
        }
        if (bracket_count < 0)
        {
            return s.substr(pos, i - 1 - pos);
        }
        switch (s[i])
        {
            case '(':
                ++bracket_count;
                break;
            case ')':
                --bracket_count;
                break;
            case '<':
                ++angle_count;
                break;
            case '>':
                --angle_count;
                break;
            case '[':
                ++square_count;
                break;
            case ']':
                --square_count;
                break;
            case '{':
                ++brace_count;
                break;
            case '}':
                --brace_count;
                break;
        }
    }

    return s.substr(pos);
}

}

#endif // _OPERATION_LOG_FUNCTION_INFO_H