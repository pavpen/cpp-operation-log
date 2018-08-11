---
layout: post
title:  "Log Macros"
date:   2018-02-27 1:00:00
categories: examples plain-text-log
---

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


## Example

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
