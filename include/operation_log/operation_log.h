#ifndef _OPERATION_LOG_OPERATION_LOG_H
#define _OPERATION_LOG_OPERATION_LOG_H

#include <functional>
#include <ostream>
#include <stack>

#include "function_info.h"
#include "predicate.h"


namespace operation_log
{

// A class that accepts operation log messages, formats them and writes them
// to an output stream.
template <class Formatter, class MessageFilterPredicate>
class OperationLog
{
private:
    Formatter *formatter;
    std::reference_wrapper<MessageFilterPredicate> message_filter_predicate;
    std::stack<FunctionInfo> call_stack;

public:
    OperationLog(Formatter &formatter, MessageFilterPredicate &message_filter_predicate)
    : formatter(&formatter),
    message_filter_predicate(message_filter_predicate)
    {}

public:
    Formatter& get_formatter()
    {
        return *formatter;
    }

    void set_formatter(Formatter& value)
    {
        formatter = &value;
    }

    MessageFilterPredicate& get_message_filter_predicate()
    {
        return message_filter_predicate.get();
    }

    void set_message_filter_predicate(MessageFilterPredicate &value)
    {
        message_filter_predicate = value;
    }

    std::ostream& get_output_stream()
    {
        return formatter->get_output_stream();
    }

    void set_output_stream(std::ostream &value)
    {
        formatter->set_output_stream(value);
    }

    void write_message(std::string message)
    {
        if (message_filter_predicate.get()(call_stack))
        {
            formatter->write_message(message);
        }
    }

    template <typename... VarTs>
    void dump_vars(const std::vector<std::string> &names, VarTs... vars)
    {
        if (message_filter_predicate.get()(call_stack))
        {
            formatter->dump_vars(names, vars...);
        }
    }

    template <typename... ArgTs>
    void log_function_entry(FunctionInfo &function_info, ArgTs... args)
    {
        call_stack.push(function_info);
        if (message_filter_predicate.get()(call_stack))
        {
            formatter->log_function_entry(function_info, args...);
        }
    }

    void enter_function()
    {
        formatter->enter_function();
    }

    void exit_function()
    {
        formatter->exit_function();
        call_stack.pop();
    }
};

}

#endif // _OPERATION_LOG_OPERATION_LOG_H
