#ifndef _OPERATION_LOG_OPERATION_LOG_INSTANCE_H
#define _OPERATION_LOG_OPERATION_LOG_INSTANCE_H

#include <iostream>
#include <stack>

#include "operation_log.h"
#include "plain_text_formatter.h"
#include "predicate.h"

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
    OperationLog<PlainTextFormatter, RunTimePredicate<const std::stack<FunctionInfo>&>>
        log;

public:
    static OperationLog<PlainTextFormatter, RunTimePredicate<const std::stack<FunctionInfo>&>>& get()
    {
        static OperationLogInstance instance_builder;

        return instance_builder.log;
    }

    OperationLogInstance()
    : log(std::cout, message_filter)
    {
#ifdef OPERATION_LOG_INIT_CODE
        OPERATION_LOG_INIT_CODE
#endif // OPERATION_LOG_INIT_CODE
    }
};

}

#endif // _OPERATION_LOG_OPERATION_LOG_INSTANCE_H
