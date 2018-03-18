#ifndef _OPERATION_LOG_HTML_UTILS_H
#define _OPERATION_LOG_HTML_UTILS_H

#include <ostream>
#include <sstream>
#include <string>


namespace operation_log
{

class HtmlUtils
{
    public:

    static void write_escaped(std::ostream &out, std::string value)
    {
        for (char ch : value)
        {
            switch (ch)
            {
                case '&':  out << "&amp;";       break;
                case '\"': out << "&quot;";      break;
                case '\'': out << "&apos;";      break;
                case '<':  out << "&lt;";        break;
                case '>':  out << "&gt;";        break;
                default:   out << ch;            break;
            }
        }
    }

    static std::string escape(std::string value)
    {
        std::stringstream res;

        write_escaped(res, value);

        return res.str();
    }
};

}

#endif // _OPERATION_LOG_HTML_UTILS_H
