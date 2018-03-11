// Include this file to enable operation logging in code that follows.

#ifdef OPERATION_LOG_ENABLE

#undef OPERATION_LOG_ENTER_NO_ARG_FUNCTION
#undef OPERATION_LOG_ENTER_FUNCTION
#undef OPERATION_LOG_LEAVE_FUNCTION
#undef OPERATION_LOG_DUMP_VARS
#undef OPERATION_LOG_MESSAGE
#undef OPERATION_LOG_MESSAGE_STREAM


#define OPERATION_LOG_ENTER_NO_ARG_FUNCTION() \
        operation_log::FunctionEntry OPERATION_LOG_FUNCTION_VAR_NAME(__PRETTY_FUNCTION__);

#define OPERATION_LOG_ENTER_FUNCTION(...) \
        operation_log::FunctionEntry OPERATION_LOG_FUNCTION_VAR_NAME(__PRETTY_FUNCTION__, #__VA_ARGS__, __VA_ARGS__);

#define OPERATION_LOG_LEAVE_FUNCTION()  OPERATION_LOG_FUNCTION_VAR_NAME.exit_function();

#define OPERATION_LOG_DUMP_VARS(...) \
        operation_log::OperationLogInstance::get().dump_vars(\
            operation_log::CppParsing::parse_stringified_list(#__VA_ARGS__), \
            __VA_ARGS__);

#define OPERATION_LOG_MESSAGE(msg)  \
        operation_log::OperationLogInstance::get().write_message(msg);

#define OPERATION_LOG_MESSAGE_STREAM(args)  \
        { operation_log::MessageStream() args; }

#else // OPERATION_LOG_ENABLE

#include <operation_log/disable.h>

#endif // OPERATION_LOG_ENABLE
