#ifndef _OPERATION_LOG_FORWARD_DECLARATIONS_H
#define _OPERATION_LOG_FORWARD_DECLARATIONS_H

#include <stack>

namespace operation_log
{

class FunctionInfo;

class FormatterBase;

template <typename... ArgTs>
class RunTimePredicate;

template <class Formatter, class MessageFilterPredicate>
class OperationLog;

typedef
    OperationLog<
        FormatterBase,
        RunTimePredicate<
            const std::stack<FunctionInfo>&
        >
    >
    DefaultOperationLog;

}

#endif // _OPERATION_LOG_FORWARD_DECLARATIONS_H