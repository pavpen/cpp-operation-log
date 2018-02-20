#ifndef _OPERATION_LOG_CPP_PARSING_H
#define _OPERATION_LOG_CPP_PARSING_H

#include <string>
#include <vector>

namespace operation_log
{

class CppParsing
{
public:
    static std::vector<std::string> parse_stringified_list(std::string stringified_list)
    {
        std::vector<std::string> res;
        std::size_t i;
        std::size_t len = stringified_list.length();

        for (i = 0; i < len; ++i)
        {
            std::size_t name_end = std::min(stringified_list.find(',', i), len);

            res.emplace_back(stringified_list.substr(i, name_end - i));
            i = name_end + 1;
        }

        return res;
    }
};

}

#endif // _OPERATION_LOG_CPP_PARSING_H
