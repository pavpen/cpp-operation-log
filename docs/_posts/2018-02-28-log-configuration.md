---
layout: post
title:  "Log Configuration"
date:   2018-02-28 1:00:00
categories: examples configuration
---

## Compile Logging In, or Out

To enable logging at compile time, define the `OPERATION_LOG_ENABLE` macro, or
just undefine the `NDEBUG` macro.  To disable logging at compile time, whether
`NDEBUG` is defined, or not, define the `OPERATION_LOG_DISABLE` macro.


## Run Time Logger Configuration

You can configure the logger using a C++ function executed at log
instantiation, or by assigning code to a macro (ugly, and not recommended).
In the first case you need to specify the function name and namespace before
you include the `operation_log.h` header file for the first time by defining
macros.  You can typically do that by putting the configuration code and
in a `config.h`.


### Using a Configuration Function

```C++
// Configure operation log initalization function name and namespace before
// the first <operation_log.h> include statement:
#define OPERATION_LOG_INIT_FUNCTION_NAMESPACE  output_sphere_config
#define OPERATION_LOG_INIT_FUNCTION_NAME       operation_log_init

#include <operation_log.h>


// Define the operation log initialization function:
namespace output_sphere_config
{

void operation_log_init(operation_log::DefaultOperationLog &log)
{
    // Define a function for selecting what messages get logged:
    class MessageFilter : public operation_log::RunTimePredicate<const std::stack<operation_log::FunctionInfo>&>
    {
    public:
        bool operator()(const std::stack<operation_log::FunctionInfo>& call_stack)
        {
            const std::string func_name = call_stack.top().get_short_name();

            return func_name == "advance_prev_parallel_vertex" ||
                func_name == "add_vertex";
        }
    };

    // Make sure the message_filter instance isn't destroyied when this
    // function returns:
    static MessageFilter message_filter;

    log.set_message_filter_predicate(message_filter);

    // Output an HTML log:
    static std::ofstream output_stream("operation-log.html");
    static operation_log::HtmlFormatter formatter(output_stream, "MyApp's Operation Log");

    // Enable Three.js in the HTML log:
    formatter.extra_header_code =
        operation_log::HtmlFormatter::three_js_header_code;

    log.set_formatter(formatter);
}

}
```


### Using Ugly Configuration Code Assignment to a Macro

```C++
// Operation log configuration code before the first <operation_log.h> include:
#define OPERATION_LOG_INIT_CODE  \
    // Only log messages from the `advance_prev_parallel_vertex` and \
    // `add_vertex` functions: \
    class MessageFilter : public RunTimePredicate<const std::stack<FunctionInfo>&> \
    { \
    public: \
        bool operator()(const std::stack<FunctionInfo>& call_stack) \
        { \
            const std::string func_name = call_stack.top().get_short_name(); \
            \
            return func_name == "advance_prev_parallel_vertex" || \
                func_name == "add_vertex"; \
        } \
    }; \
    \
    static MessageFilter message_filter; \
    \
    log.set_message_filter_predicate(message_filter);


#include <operation_log.h>
```
