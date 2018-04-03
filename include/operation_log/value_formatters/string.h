#ifndef _OPERATION_LOG_VALUE_FORMATTERS_STRING_H
#define _OPERATION_LOG_VALUE_FORMATTERS_STRING_H

#include "../value_formatter_base.h"
#include "../value_formatter_i.h"


namespace operation_log
{

// Class specialization for `std::string`s.  I.e. default formatter for
// `std::string` objects.
template <>
class ValueFormatterBase<std::string> : public ValueFormatterI
{
	public:

	const std::string value;

	ValueFormatterBase(std::string value)
	: value(value)
	{}

	std::string to_text() override
	{
		std::stringstream res;

		res << '"';
		for (const auto &ch : value)
		{
			switch (ch)
			{
				case '"':
					res << "\\\"";
					break;
				case '\r':
					res << "\\\r";
					break;
				case '\n':
					res << "\\\n";
					break;
				default:
					res << ch;
			}
		}
		res << '"';

		return res.str();
	}

	std::string to_html() override
	{
		return HtmlUtils::escape(to_text());
	}
};

}

#endif // _OPERATION_LOG_VALUE_FORMATTERS_STRING_H
