#ifndef _OPERATION_LOG_VALUE_FORMATTER_BASE_H
#define _OPERATION_LOG_VALUE_FORMATTER_BASE_H

#include <string>

#include "html_utils.h"
#include "type_traits.h"

#include "value_formatter_i.h"


namespace operation_log
{

// A base type for formatting variable values.
template <typename T>
class ValueFormatterBase : public ValueFormatterI
{
    protected:

	const T value;
	typedef typename HasToString<T>::has_trait_type ValueHasToString;
	typedef typename HasOstreamRShift<T>::has_trait_type ValueHasOstreamRShift;

	public:

	ValueFormatterBase(const T value)
	: value(value)
	{}

	std::string to_text() override
	{
		return to_string(
			value, ValueHasToString(), ValueHasOstreamRShift());
	}

	std::string to_html() override
	{
		return HtmlUtils::escape(to_string(
			value, ValueHasToString(), ValueHasOstreamRShift() ));
	}

    protected:

	template <typename HasOstreamRShift>
	std::string to_string(const T &value, std::true_type has_to_string, HasOstreamRShift)
	{
		return std::to_string(value);
	}

	std::string to_string(
		const T &value, std::false_type has_to_string,
		std::true_type has_ostream_rshift)
	{
		std::stringstream res;

		res << value;

		return res.str();
	}

	// std::string to_string(
	// 	const T &value, std::false_type has_to_string,
	// 	std::false_type has_ostream_rshift)
	// {
	// 	std::cout << "No conversion to string." << std::endl;
	// }
};

// Class specialization for `std::string`s:
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

#endif // _OPERATION_LOG_VALUE_FORMATTER_BASE_H
