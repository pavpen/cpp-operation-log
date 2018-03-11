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

#ifndef OPERATION_LOG_FUNCTION_VAR_NAME
#    define OPERATION_LOG_FUNCTION_VAR_NAME operation_log__function
#endif // OPERATION_LOG_FUNCTION_VAR_NAME


// Unused:
#define OPERATION_LOG_ARGUMENT_COUNT(...) \
    (std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value)

#ifdef OPERATION_LOG_ENABLE
#    include <operation_log/enable.h>
#else // OPERATION_LOG_ENABLE
#    include <operation_log/disable.h>
#endif // OPERATION_LOG_ENABLE

#endif // _OPERATION_LOG_H