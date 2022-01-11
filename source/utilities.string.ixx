module;

#include <cctype> // std::isdigit, std::tolower, ...
#include <string_view>
#include <format>
#include <charconv> //'std::from_chars
#include <stdexcept> // std::runtime_error

export module utilities.string;

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace str //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
{


//---------------------------------------------------------------------------
// Convert string_view to int
export int to_int(const std::string_view s)
{
    int result;
    auto [p, ec] = std::from_chars(s.data(), s.data()+s.size(), result);
    //if(ec == std::errc::runtime_error || ec == std::errc::result_out_of_range)
    if( ec != std::errc() || p!=(s.data()+s.size()) ) throw std::runtime_error(std::format("\"{}\" is not a valid integer",s));
    return result;
}


}//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

