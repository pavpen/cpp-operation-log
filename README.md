# lib Operation Log

A C++ library for logging debug messages that can include complex objects,
such as 3D models, matrices, and other user-defined entities.

It has a basic plain-text formatter, and an HTML formatter which can be used
to log raw HTML messages (e.g., you can send 3D scenes to you log).  Other
formatters can be added.

## Features

You can:

* Log function entry and exit.
* Write a custom function to filter what messages get written to the log.
* Write formatters for objects of any custom class, or override pre-defined
  formatters for common object types.
* Compile out logging based on a macro definition.
* Temporarily disable logging for a section of source code (such as an include
  file, or a class).
* Dump variables.
* Construct messages using the `std::ostream` style of overloading the `<<`
  operator.
* Switch the output format, output file, or filter function at run-time (e.g.,
  based on a configuration file).


## Example

### HTML Example

Browse one
[here](https://pavpen.github.io/cpp-operation-log/2018/html-log-sphere-tesselation.html).


### Source Sample

You can use macros such as the following:

```C++
OPERATION_LOG_ENTER_NO_ARG_FUNCTION()

OPERATION_LOG_ENTER_FUNCTION(var1, var2)

OPERATION_LOG_LEAVE_FUNCTION()

OPERATION_LOG_DUMP_VARS(var1, var2)

OPERATION_LOG_MESSAGE("1/1/1: There was a world.")

OPERATION_LOG_MESSAGE_STREAM(<<
    "This is like std::ostream: " << var1 << ", " << var2 << ", " << var3)

OPERATION_LOG_MESSAGE_STREAM_OPEN(log_msg)

OPERATION_LOG_MESSAGE_STREAM_WRITE(log_msg, << "Construct message step by step,")
OPERATION_LOG_MESSAGE_STREAM_WRITE(log_msg, << "e.g. in a loop.")

// Write the constructed message to the log:
OPERATION_LOG_MESSAGE_STREAM_CLOSE(log_msg)

OPERATION_LOG_CODE(
    // Code only compiled when operation logging is enabled:
    int vertex_count;
    // . . .
    ++vertex_count;
)

```

Here's a verbose example:

```C++
#include <operation_log.h>

template <class HDS>
class Sphere_3_TessalationBuilder : public CGAL::Modifier_base<HDS>
{
private:
    OPERATION_LOG_CODE(
        int vertex_count;
    )

    inline void advance_prev_parallel_vertex()
    {
        OPERATION_LOG_ENTER_NO_ARG_FUNCTION();

        // . . . code . . .

        if (next_vertex_i != prev_parallel_vertex_i)
        {
            OPERATION_LOG_MESSAGE_STREAM(<<
                "Face vertices: " << prev_parallel_vertex_i <<
                ", " << parallel_vertex_i << ", " << next_vertex_i)

            // . . . code . . .
        }
        else
        {
            OPERATION_LOG_MESSAGE("Previous parallel has < 2 subdivisions. It's already complete.");
        }

        // . . . code . . .

        OPERATION_LOG_DUMP_VARS(prev_parallel_vertex_i, longitude_difference_subdiv);
        OPERATION_LOG_LEAVE_FUNCTION();
    }

    inline void add_vertex(double latitude, double longitude)
    {
        OPERATION_LOG_ENTER_FUNCTION(latitude / M_PI, longitude / M_PI);

        // . . . code . . .

        OPERATION_LOG_MESSAGE_STREAM(<<
            "Vertex " << vertex_count << ": " << point);

        // . . . code . . .

        OPERATION_LOG_CODE(
            vertex_count++;
        )

        OPERATION_LOG_LEAVE_FUNCTION();
    }
};
```

### Log Sample


```
void add_vertex(double latitude / M_PI=0.5, double longitude / M_PI=0)
  Vertex 0: 6.12323e-16 0 10
  void add_vertex(double latitude / M_PI=-0.166667, double longitude / M_PI=0)
    Vertex 1: 8.66025 0 -5
    void add_vertex(double latitude / M_PI=-0.166667, double longitude / M_PI=0.666667)
      Vertex 2: -4.33013 7.5 -5
    void add_vertex(double latitude / M_PI=-0.166667, double longitude / M_PI=1.33333)
      Vertex 3: -4.33013 -7.5 -5
  void advance_prev_parallel_vertex()
    Previous parallel has < 2 subdivisions. It's already complete.
    prev_parallel_vertex_i = 1,
longitude_difference_subdiv = 1
```

## Logger Configuration

You can configure the logger statically.

### Using a Configuration Function

```C++
#define OPERATION_LOG_INIT_FUNCTION_NAMESPACE  output_sphere_config
#define OPERATION_LOG_INIT_FUNCTION_NAME       operation_log_init

#include <operation_log.h>

namespace output_sphere_config
{

void operation_log_init(operation_log::DefaultOperationLog &log)
{
    // Filter operation log messages:
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

### Using Ugly Static Configuration Code

```C++
// Operation log:
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


## Write Value Formatters for Custom Data Types

If you want to dump variables with custom data types to the operation log,
their values need to be convertable to strings.  The precedence for converting
a `custom_value` to strings is the following.  (`decltype(custom_value)` means
the data type of the `custom_value` variable.)

1. `ValueFormatter<decltype(custom_value)>`.
1. `std::to_string(custom_value)`.
1. `std::cout << custom_value`.

To define a value formatter for a custom data type, or to override a default
formatter, implement a class specialization like the following.

```C++
namespace operation_log
{

// A value formatter for the `cpp_cad::Polygon_2` data type.
template <>
class ValueFormatter<cpp_cad::Polygon_2> : public ValueFormatterI
{
    private:

    const cpp_cad::Polygon_2 value;

    public:

    // Receives the value to format.
    ValueFormatter(const cpp_cad::Polygon_2 value)
    : value(value)
    {}

    // Formats values for plain text logs.
    std::string to_text() override
    {
        std::stringstream res;

        res << "Polygon_2 { ";
        typename cpp_cad::Polygon_2::Vertex_const_iterator vit;
        bool is_first = true;

        for (vit = value.vertices_begin(); vit != value.vertices_end(); ++vit)
        {
            if (is_first)
            {
                is_first = false;
            }
            else
            {
                res << ", ";
            }
            res << (*vit);
        }
        res << " }";

        return res.str();
    }

    // Formats values for HTML logs.
    std::string to_html() override
    {
        // We can output a Three.js scene, or an SVG element here.
        return HtmlUtils::escape(to_text());
    }
};

}
```


## Requirements

* C++11 or later.
* Tested only with GCC.


## Installation

### Ubuntu 16

```BASH
./build_packages.sh
sudo dpkg -i build/liboperationlog_0.1.0.deb
```


## Compile Logging In, or Out

To enable logging at compile time, define the `OPERATION_LOG_ENABLE` macro, or
just undefine the `NDEBUG` macro.  To disable logging at compile time, whether
`NDEBUG` is defined, or not, define the `OPERATION_LOG_DISABLE` macro.
