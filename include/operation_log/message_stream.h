#ifndef _OPERATION_LOG_MESSAGE_STREAM_H
#define _OPERATION_LOG_MESSAGE_STREAM_H

#include <sstream>

#include "operation_log_instance.h"

namespace operation_log
{

// Creates an output stream, which writes its contents as an operation log
// message before it's destroyed.
//
// You can use this class to format log messages as you would format output
// to an `std::ostream`.
class MessageStream : public std::stringstream
{
    private:

    bool is_closed = false;

    public:

    ~MessageStream()
    {
        close();
    }

    void close()
    {
        if (!is_closed)
        {
            OperationLogInstance::get().write_message(str());
            is_closed = true;
        }
    }
};

}

#endif // _OPERATION_LOG_MESSAGE_STREAM_H
