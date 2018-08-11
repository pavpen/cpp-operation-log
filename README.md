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
* Switch configuration at run time (e.g., based on a configuration file), such as:
    * The log output format (e.g. plain text or HTML),
    * The output file path,
    * The filter function for selecting what messages get logged.


## Example

```C++
#include <operation_log.h>

template <class HDS>
class Sphere_3_TessalationBuilder : public CGAL::Modifier_base<HDS>
{
private:
    OPERATION_LOG_CODE(
        // This code gets compiled only when logging is compiled in:
        int vertex_count;
    )

    inline void advance_prev_parallel_vertex()
    {
        // Log the function entry:
        OPERATION_LOG_ENTER_NO_ARG_FUNCTION();

        // . . . code . . .

        if (next_vertex_i != prev_parallel_vertex_i)
        {
            // You can construct log messages using std::ostream syntax.
            // You can define formatters for logging values of custom data
            // types.
            OPERATION_LOG_MESSAGE_STREAM(<<
                "Face vertices: " << prev_parallel_vertex_i <<
                ", " << parallel_vertex_i << ", " << next_vertex_i)

            // . . . code . . .
        }
        else
        {
            // You can log an already formatted text message:
            OPERATION_LOG_MESSAGE("Previous parallel has < 2 subdivisions. It's already complete.");
        }

        // . . . code . . .

        // You can log variables by listing their names:
        OPERATION_LOG_DUMP_VARS(prev_parallel_vertex_i, longitude_difference_subdiv);
        // You can explicitly log function exit.  (Otherwise, it will be inferred.)
        OPERATION_LOG_LEAVE_FUNCTION();
    }

    inline void add_vertex(double latitude, double longitude)
    {
        // You can log the parameter values that a function was called with:
        OPERATION_LOG_ENTER_FUNCTION(latitude / M_PI, longitude / M_PI);

        // . . . code . . .

        OPERATION_LOG_MESSAGE_STREAM(<<
            "Vertex " << vertex_count << ": " << point);

        // . . . code . . .

        // You can execute data tracking code only when logging is compiled in:
        OPERATION_LOG_CODE(
            vertex_count++;
        )

        OPERATION_LOG_LEAVE_FUNCTION();
    }
};
```

### HTML Example

Browse one
[here](https://pavpen.github.io/cpp-operation-log/2018/html-log-sphere-tesselation.html).


## Requirements

* C++11 or later.
* Tested only with GCC.


## Installation

### Ubuntu 16

```BASH
./build_packages.sh
sudo dpkg -i build/liboperationlog_0.1.0.deb
```

## Further Documentation

https://pavpen.github.io/cpp-operation-log/
