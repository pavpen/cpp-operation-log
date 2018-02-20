#ifndef _OPERATION_LOG_FUNCTION_ENTRY_H
#define _OPERATION_LOG_FUNCTION_ENTRY_H

#include "function_info.h"
#include "operation_log_instance.h"

namespace operation_log
{

// A class that logs entry into and exit from a C++ function.
//
// ## Implementation details:
//
// An object of this class is created upon function entry.  The constructor
// logs a function entry message.
//
// The destructor logs a function exit message, so the block this object is
// created in cannot be exited without a function exit message's being logged.
//
// However, a destructor can be called earlier than the end of the block (as
// soon as the object isn't going to be used any more in that block).
// Therefore, we need to postpone destroying the object until the end of the
// block is reached.  (Otherwise, a function return will be reported early.)
//
// For that purpose, the `exit_function()` should be called just before
// returning from a fuction.  It will prevent the destructor from being called
// early.
class FunctionEntry
{
public:
    FunctionEntry(std::string pretty_function)
    : function_info(pretty_function, "")
    {
        OperationLogInstance::get().log_function_entry(function_info);
        OperationLogInstance::get().enter_function();
    }

    template <typename... ArgTs>
    FunctionEntry(
        std::string pretty_function, std::string stringified_arg_list,
        ArgTs... args)
    : function_info(pretty_function, stringified_arg_list)
    {
        OperationLogInstance::get().log_function_entry(
            function_info, args...);
        OperationLogInstance::get().enter_function();
    }

    ~FunctionEntry()
    {
        OperationLogInstance::get().exit_function();
    }

    // This method is used just to keep the object from being destroyed until
    // the end of the function block has been reached.
    void exit_function()
    {
        static volatile bool leaving_function;

        leaving_function = true;
    }

private:
    FunctionInfo function_info;
};

}

#endif // _OPERATION_LOG_FUNCTION_ENTRY_H
