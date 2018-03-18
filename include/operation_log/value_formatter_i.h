#ifndef _OPERATION_LOG_VALUE_FORMATTER_I_H
#define _OPERATION_LOG_VALUE_FORMATTER_I_H

#include <string>


namespace operation_log
{

class ValueFormatterI
{
    public:

    virtual std::string to_text() = 0;
    virtual std::string to_html() = 0;
};

}

#endif // _OPERATION_LOG_VALUE_FORMATTER_I_H
