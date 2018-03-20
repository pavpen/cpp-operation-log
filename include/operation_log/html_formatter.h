#ifndef _OPERATION_LOG_HTML_FORMATTER_H
#define _OPERATION_LOG_HTML_FORMATTER_H

#include <functional>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

#include "formatter_base.h"
#include "function_info.h"
#include "html_utils.h"
#include "value_formatter_i.h"


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

public:
    std::string extra_header_code = "";
    static const std::string three_js_header_code;
    static const std::string math_jax_header_code;

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
        HtmlUtils::write_escaped(output.get(), value);
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
            style_code <<
            extra_header_code <<
            R"code(
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

    void write_html_value(std::string code) override
    {
        output.get() << "  <div class=\"operation-log-html-message\">" <<
            code <<
            "</div>" << std::endl;
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

    void write_dump_var_lm(std::string name, std::string value_formatted)
    {
        output.get() <<
            "    <div class=\"operation-log-var\"><span class=\"operation-log-var-name\">";
        write_escaped(name);
        output.get() << "</span> = <span class=\"operation-log-var-value\">" <<
            value_formatted;
        output.get() << "</span></div>" << std::endl;
    }

    void write_dump_var(std::string name, ValueFormatterI &value_formatter)
    {
        write_dump_var_lm(name, value_formatter.to_html());
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
        std::string value_formatted)
    {
        output.get() << "<span class=\"operation-log-function-arg-type\">";
        write_escaped(type_name);
        output.get() << "</span> <span class=\"operation-log-function-arg-name\">";
        write_escaped(parameter_name);
        output.get() << "</span> = <span class=\"operation-log-function-arg-value\">" <<
            value_formatted;
        output.get() << "</span>";
    }

    void write_function_arg(
        std::string type_name, std::string parameter_name, ValueFormatterI &value_formatter)
    {
        write_function_arg_lms(type_name, parameter_name, value_formatter.to_html());
    }

    void write_function_extra_info(std::string info) override
    {
        output.get() << " <span class=\"operation-log-function-extra-info\">";
        write_escaped(info);
        output.get() << "</span>";
    }
};

const std::string HtmlFormatter::three_js_header_code =
#include "html_formatter/three_js.h"
;

const std::string HtmlFormatter::math_jax_header_code =
    "<script src='https://cdnjs.cloudflare.com/ajax/libs/mathjax/2.7.2/MathJax.js?config=TeX-MML-AM_CHTML'></script>\n";


}

#endif // _OPERATION_LOG_HTML_FORMATTER_H