module;
//  ---------------------------------------------
//  conversions utilities
//  ---------------------------------------------
#include <string_view>
#include <format>
#include <expected>
#include <charconv> // std::from_chars
#include <chrono>
#include <sstream>
#include <string>

export module utilities.conversions;

namespace conv //::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
{

//---------------------------------------------------------------------------
// Convert a string_view to number
export
template<typename T>
[[nodiscard]] constexpr std::expected<T, std::string> to_num(const std::string_view sv) noexcept
{
    T result;
    const auto it_end = sv.data() + sv.size();
    const auto [it, ec] = std::from_chars(sv.data(), it_end, result);
    if( ec!=std::errc() or it!=it_end )
       {
        return std::unexpected( std::format("\"{}\" is not a valid number", sv) );
       }
    return result;
}

//---------------------------------------------------------------------------
// Convert a string_view to date
export
[[nodiscard]] std::expected<std::chrono::year_month_day,std::string> to_date(const std::string_view sv, const std::string& fmt) noexcept
{
    std::chrono::year_month_day date;
    // Unfortunately 'parse' needs a stream, that can't be constructed with a 'string_view'
    const std::string s{sv};
    std::istringstream iss(s);
    if( not (iss >> std::chrono::parse(fmt, date)) )
        {
         return std::unexpected( std::format("\"{}\" is not a valid date", sv) );
        }
    return date;
}


}//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
