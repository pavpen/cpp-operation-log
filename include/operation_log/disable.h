// Include this file to disable operation logging in code that follows.

// Operation logging is disabled for the current code section:
#undef OPERATION_LOG
#undef OPERATION_LOG_ENTER_NO_ARG_FUNCTION
#undef OPERATION_LOG_ENTER_FUNCTION
#undef OPERATION_LOG_LEAVE_FUNCTION
#undef OPERATION_LOG_DUMP_VARS
#undef OPERATION_LOG_MESSAGE
#undef OPERATION_LOG_MESSAGE_STREAM

#define OPERATION_LOG_ENTER_NO_ARG_FUNCTION()
#define OPERATION_LOG_ENTER_FUNCTION(...)
#define OPERATION_LOG_LEAVE_FUNCTION()
#define OPERATION_LOG_DUMP_VARS(...)
#define OPERATION_LOG_MESSAGE(...)
#define OPERATION_LOG_MESSAGE_STREAM(...)
