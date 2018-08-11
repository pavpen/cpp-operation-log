---
layout: post
title:  "Value Formatters for Custom Data Types"
date:   2018-03-01 1:00:00
categories: examples configuration value-formatters
---


If you want to dump variables with custom data types to the operation log,
their values need to be convertable to strings (which get sent to the logs).
The precedence for converting a `custom_value` to a string is the following
(`decltype(custom_value)` means the data type of the `custom_value` variable):

1. `ValueFormatter<decltype(custom_value)>`.
1. `std::to_string(custom_value)`.
1. `std::cout << custom_value`.

To define a value formatter for a custom data type, or to override a default
formatter, implement a class specialization of `ValueFormatter` like the
following.


## Example

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
        // We could output a Three.js scene, or an SVG element here.
        return HtmlUtils::escape(to_text());
    }
};

}
```

