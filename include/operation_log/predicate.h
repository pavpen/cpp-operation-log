#ifndef _OPERATION_LOG_PREDICATE_H
#define _OPERATION_LOG_PREDICATE_H

namespace operation_log
{

template <typename... ArgTs>
class RunTimePredicate
{
public:
    virtual bool operator()(ArgTs... args) = 0;
};

}

#endif // _OPERATION_LOG_PREDICATE_H
