# lib Operation Log

A small C++ library for logging debug messages, such as function entry and
exit, dumping variables, and creating `std::ostream` messages.

It has a basic plain-text formatter.  Others can be added.

## Requirements

* C++11 or later.
* Tested only with GCC.

## Installation

### Ubuntu 16

```BASH
./build_packages.sh
sudo dpkg -i build/liboperationlog_0.1.0.deb
```

## Example

### Source Sample

You can use macros such as the following:

```C++
OPERATION_LOG_ENTER_NO_ARG_FUNCTION(oplog_func)

OPERATION_LOG_ENTER_FUNCTION(oplog_func, var1, var2)

OPERATION_LOG_LEAVE_FUNCTION(oplog_func)

OPERATION_LOG_DUMP_VARS(var1, var2)

OPERATION_LOG_MESSAGE("1/1/1: There was a world.")

OPERATION_LOG_MESSAGE_STREAM(<<
    "This is like std::ostream: " << var1 << ", " << var2 << ", " << var3)

```

Here's a verbose example:

```C++
template <class HDS>
class Sphere_3_TessalationBuilder : public CGAL::Modifier_base<HDS>
{
private:
    inline void advance_prev_parallel_vertex()
    {
        OPERATION_LOG_ENTER_NO_ARG_FUNCTION(oplog_func);

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
        OPERATION_LOG_LEAVE_FUNCTION(oplog_func);
    }

    inline void add_vertex(double latitude, double longitude)
    {
        OPERATION_LOG_ENTER_FUNCTION(oplog_func, latitude / M_PI, longitude / M_PI);

        double longitude_circle_r = circumsphere_r * cos(latitude);
        Kernel::Point_3 point(
                longitude_circle_r * cos(longitude),
                longitude_circle_r * sin(longitude),
                circumsphere_r * sin(latitude)
            );
        
        OPERATION_LOG_MESSAGE_STREAM(<<
            "Vertex " << vertex_count << ": " << point);

        builder.add_vertex(point);
        vertex_count++;

        OPERATION_LOG_LEAVE_FUNCTION(oplog_func);
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

### Ugly Static Configuration

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
