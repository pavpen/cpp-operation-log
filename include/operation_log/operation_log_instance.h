#ifndef _OPERATION_LOG_OPERATION_LOG_INSTANCE_H
#define _OPERATION_LOG_OPERATION_LOG_INSTANCE_H

#include <iostream>
#include <stack>

#include "forward_declarations.h"
#include "operation_log.h"
#include "plain_text_formatter.h"
#include "predicate.h"


// Forward declaration of an initialization function for the default operation
// log.  User code can define the implementation later.
#ifdef OPERATION_LOG_INIT_FUNCTION_NAME
#    ifdef OPERATION_LOG_INIT_FUNCTION_NAMESPACE
namespace OPERATION_LOG_INIT_FUNCTION_NAMESPACE
{
    void OPERATION_LOG_INIT_FUNCTION_NAME(operation_log::DefaultOperationLog &log);
}
#    else // OPERATION_LOG_INIT_FUNCTION_NAMESPACE
void OPERATION_LOG_INIT_FUNCTION_NAME(operation_log::DefaultOperationLog &log);
#    endif // OPERATION_LOG_INIT_FUNCTION_NAMESPACE
#endif // OPERATION_LOG_INIT_FUNCTION_NAME

namespace operation_log
{

class DefaultMessageFilter : public RunTimePredicate<const std::stack<FunctionInfo>&>
{
public:
    bool operator()(const std::stack<FunctionInfo> &call_stack)
    {
        return true;
    }
};

// A class that can return a global instance of an operation log.
//
// The returned global instance is used by all operation log macros.  I.e.,
// this is where log messages go during usual usage.
//
// You can configure the creation of this global instance by defining the
// `OPERATION_LOG_INIT_CODE` macro before including this header file.
class OperationLogInstance
{
private:
    DefaultMessageFilter message_filter;
    PlainTextFormatter formatter;
    DefaultOperationLog log;

public:
    static DefaultOperationLog& get()
    {
        static OperationLogInstance instance_builder;

        return instance_builder.log;
    }

    OperationLogInstance()
    : formatter(std::cout),
    log(formatter, message_filter)
    {
#ifdef OPERATION_LOG_INIT_CODE
        // Run the user-defined initialization code:
        OPERATION_LOG_INIT_CODE
#endif // OPERATION_LOG_INIT_CODE

#ifdef OPERATION_LOG_INIT_FUNCTION_NAME
        // Call the user-defined initialization function:
#    ifdef OPERATION_LOG_INIT_FUNCTION_NAMESPACE
        OPERATION_LOG_INIT_FUNCTION_NAMESPACE::OPERATION_LOG_INIT_FUNCTION_NAME(log);
#    else // OPERATION_LOG_INIT_FUNCTION_NAMESPACE
        OPERATION_LOG_INIT_FUNCTION_NAME(log);
#    endif // OPERATION_LOG_INIT_FUNCTION_NAMESPACE
#endif // OPERATION_LOG_INIT_FUNCTION_NAME
    }
};

}

#endif // _OPERATION_LOG_OPERATION_LOG_INSTANCE_H
