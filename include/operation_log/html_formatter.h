#ifndef _OPERATION_LOG_HTML_FORMATTER_H
#define _OPERATION_LOG_HTML_FORMATTER_H

#include <functional>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "formatter_base.h"
#include "function_info.h"

namespace operation_log
{

// A class which receives operation log messages, formats them as HTML,
// and writes them to an `ostream`.
class HtmlFormatter : public FormatterBase
{
public:
    inline HtmlFormatter(std::ostream &output_stream, std::string log_name="")
    : FormatterBase(output_stream),
    was_header_written(false),
    log_name(log_name)
    {}

    inline ~HtmlFormatter()
    {
        write_footer();
    }

    void exit_function()
    {
        output.get() << "</div>" << std::endl;
        this->FormatterBase::exit_function();
    }

    std::string get_style_code()
    {
        return style_code;
    }

    void set_style_code(std::string html_code)
    {
        style_code = html_code;
    }

private:
    bool was_header_written;
    std::string log_name;
    std::string style_code = R"code(
  <style>
    .operation-log {
        color: #dcdcaa;
        background-color: #1e1e1e;
    }

    .operation-log-function-frame > div {
        padding-left: 1em;
    }

    .operation-log-function-frame div.operation-log-function {
        padding-left: 0em;
    }

    .operation-log-function-return-type, .operation-log-function-arg-type {
        color: #388cd6;
    }

    .operation-log-function-name {
        color: #dcb856;
    }

    .operation-log-var-dump > div {
        border-color: #dcb856;
        border-width: 1px;
        border-style: solid;
        border-radius: 0.5em;
        margin-left: -0.5em;
        padding-left: 0.5em;
        padding-right: 0.5em;
        display: inline-block;
    }
  </style>
)code";

    void write_escaped(std::string value)
    {
        std::ostream &out = output.get();

        for (char ch : value)
        {
            switch (ch)
            {
                case '&':  out << "&amp;";       break;
                case '\"': out << "&quot;";      break;
                case '\'': out << "&apos;";      break;
                case '<':  out << "&lt;";        break;
                case '>':  out << "&gt;";        break;
                default:   out << ch;            break;
            }
        }
    }

    void write_header()
    {
        output.get() << R"code(
<html>
<head>
  <title>Operation Log)code";
        if (!log_name.empty())
        {
            output.get() << " ";
            write_escaped(log_name);
        }
        output.get() << "</title>" << std::endl <<
            style_code << R"code(
</head>

<body>

<div class="operation-log">

)code";

        was_header_written = true;
    }

    void write_footer()
    {
        output.get() << R"code(
</div>

</body>

</html>
)code";
    }

    inline void write_message_prefix()
    {
        if (!was_header_written)
        {
            write_header();
        }
    }

    void write_message_value(std::string message) override
    {
        output.get() << "  <div class=\"operation-log-message\">";
        write_escaped(message);
        output.get() << "</div>" << std::endl;
    }

    void write_dump_vars_prefix() override
    {
        output.get() << R"code(
  <div class="operation-log-var-dump">
    <div>
)code";
    }

    void write_dump_vars_suffix() override
    {
        output.get() << R"code(
    </div>
  </div>
)code";
    }

    void write_dump_vars_separator() override
    {}

    void write_dump_var_lm(std::string name, std::string value_stringified)
    {
        output.get() <<
            "    <div class=\"operation-log-var\"><span class=\"operation-log-var-name\">";
        write_escaped(name);
        output.get() << "</span> = <span class=\"operation-log-var-value\">";
        write_escaped(value_stringified);
        output.get() << "</span></div>" << std::endl;
    }

    void write_dump_var(std::string name, bool value)
    {
        write_dump_var_lm(name, std::to_string(value));
    }

    void write_dump_var(std::string name, short value)
    {
        write_dump_var_lm(name, std::to_string(value));
    }

    void write_dump_var(std::string name, unsigned short value)
    {
        write_dump_var_lm(name, std::to_string(value));
    }

    void write_dump_var(std::string name, int value)
    {
        write_dump_var_lm(name, std::to_string(value));
    }

    void write_dump_var(std::string name, unsigned int value)
    {
        write_dump_var_lm(name, std::to_string(value));
    }

    void write_dump_var(std::string name, long value)
    {
        write_dump_var_lm(name, std::to_string(value));
    }

    void write_dump_var(std::string name, unsigned long value)
    {
        write_dump_var_lm(name, std::to_string(value));
    }

    void write_dump_var(std::string name, float value)
    {
        write_dump_var_lm(name, std::to_string(value));
    }

    void write_dump_var(std::string name, double value)
    {
        write_dump_var_lm(name, std::to_string(value));
    }

    void write_dump_var(std::string name, long double value)
    {
        write_dump_var_lm(name, std::to_string(value));
    }

    void write_dump_var(std::string name, void* value)
    {
        std::stringstream fmtr;

        fmtr << value;
        write_dump_var_lm(name, fmtr.str());
    }

    void write_function_prefix() override
    {
        output.get() <<
            "<div class=\"operation-log-function-frame\">" << std::endl <<
            "  <div class=\"operation-log-function\">" << std::endl;
    }

    void write_function_suffix() override
    {
        output.get() << "  </div>" << std::endl;
    }

    void write_function_exit(FunctionInfo &function_info) override
    {
        output.get() << "</div>" << std::endl;
    }

    void write_function_return_type_and_name(
        std::string return_type, std::string name) override
    {
        output.get() <<
            "<span class=\"operation-log-function-return-type\">";
        write_escaped(return_type);
        output.get() << "</span> <span class=\"operation-log-function-name\">";
        write_escaped(name);
        output.get() << "</span>";
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

    void write_function_arg_lms(
        std::string type_name, std::string parameter_name,
        std::string value_stringified)
    {
        output.get() << "<span class=\"operation-log-function-arg-type\">";
        write_escaped(type_name);
        output.get() << "</span> <span class=\"operation-log-function-arg-name\">";
        write_escaped(parameter_name);
        output.get() << "</span> = <span class=\"operation-log-function-arg-value\">";
        write_escaped(value_stringified);
        output.get() << "</span>";
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, bool value)
    {
        write_function_arg_lms(type_name, parameter_name, std::to_string(value));
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, short value)
    {
        write_function_arg_lms(type_name, parameter_name, std::to_string(value));
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, unsigned short value)
    {
        write_function_arg_lms(type_name, parameter_name, std::to_string(value));
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, int value)
    {
        write_function_arg_lms(type_name, parameter_name, std::to_string(value));
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, unsigned int value)
    {
        write_function_arg_lms(type_name, parameter_name, std::to_string(value));
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, long value)
    {
        write_function_arg_lms(type_name, parameter_name, std::to_string(value));
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, unsigned long value)
    {
        write_function_arg_lms(type_name, parameter_name, std::to_string(value));
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, float value)
    {
        write_function_arg_lms(type_name, parameter_name, std::to_string(value));
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, double value)
    {
        write_function_arg_lms(type_name, parameter_name, std::to_string(value));
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, long double value)
    {
        write_function_arg_lms(type_name, parameter_name, std::to_string(value));
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, void* value)
    {
        std::stringstream fmtr;

        fmtr << value;
        write_function_arg_lms(type_name, parameter_name, fmtr.str());
    }

    void write_function_extra_info(std::string info) override
    {
        output.get() << " <span class=\"operation-log-function-extra-info\">";
        write_escaped(info);
        output.get() << "</span>";
    }
};

}

#endif // _OPERATION_LOG_HTML_FORMATTER_H