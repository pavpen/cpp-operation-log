#ifndef _OPERATION_LOG_H
#define _OPERATION_LOG_H

#include <iostream>
#include <ostream>
#include <sstream>
#include <tuple>
#include <vector>

#include <operation_log/cpp_parsing.h>
#include <operation_log/function_entry.h>
#include <operation_log/function_info.h>
#include <operation_log/html_formatter.h>
#include <operation_log/message_stream.h>
#include <operation_log/operation_log_instance.h>
#include <operation_log/operation_log.h>
#include <operation_log/plain_text_formatter.h>


// Should we enable operation logging:
#ifndef OPERATION_LOG_ENABLE
#    ifndef DEBUG
#        define OPERATION_LOG_ENABLE
#    endif // DEBUG
#endif // OPERATION_LOG_ENABLE

#ifdef OPERATION_LOG_DISABLE
#    undef OPERATION_LOG_ENABLE
#endif // OPERATION_LOG_DISABLE


// Unused:
#define OPERATION_LOG_ARGUMENT_COUNT(...) \
    (std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value)

#ifdef OPERATION_LOG_ENABLE
#   define OPERATION_LOG_ENTER_NO_ARG_FUNCTION(var_name) \
        operation_log::FunctionEntry var_name(__PRETTY_FUNCTION__);
#   define OPERATION_LOG_ENTER_FUNCTION(var_name, ...) \
        operation_log::FunctionEntry var_name(__PRETTY_FUNCTION__, #__VA_ARGS__, __VA_ARGS__);
#   define OPERATION_LOG_LEAVE_FUNCTION(var_name)  var_name.exit_function();
#   define OPERATION_LOG_DUMP_VARS(...) \
        operation_log::OperationLogInstance::get().dump_vars(\
            operation_log::CppParsing::parse_stringified_list(#__VA_ARGS__), \
            __VA_ARGS__);
#   define OPERATION_LOG_MESSAGE(msg)  \
        operation_log::OperationLogInstance::get().write_message(msg);
#   define OPERATION_LOG_MESSAGE_STREAM(args)  \
        { operation_log::MessageStream() args; }
#else // OPERATION_LOG_ENABLE
#   define OPERATION_LOG_ENTER_NO_ARG_FUNCTION(var_name) 
#   define OPERATION_LOG_ENTER_FUNCTION(var_name, ...) 
#   define OPERATION_LOG_LEAVE_FUNCTION(var_name) 
#   define OPERATION_LOG_DUMP_VARS(...) 
#   define OPERATION_LOG_MESSAGE(msg) 
#   define OPERATION_LOG_MESSAGE_STREAM(args) 
#endif // OPERATION_LOG_ENABLE

#endif // _OPERATION_LOG_H