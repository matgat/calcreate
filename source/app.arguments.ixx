module;
//  ---------------------------------------------
//  Program arguments
//  ---------------------------------------------
#include <string_view>
#include <string>
#include <format>
#include <print>
#include <chrono>

export module app.arguments;

import utilities.conversions; // conv::to_num<>, conv::to_date

using namespace std::literals; // ""sv

namespace app //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
{

/////////////////////////////////////////////////////////////////////////////
export class Arguments
{
    std::chrono::year_month_day m_startdate;
    int m_nweeks;

 public:
    [[nodiscard]] const auto& start_date() const noexcept { return m_startdate; }
    [[nodiscard]] int weeks() const noexcept { return m_nweeks; }

    Arguments( int argc, const char* argv[], const std::string& date_fmt )
       : m_startdate{curr_date()}
       , m_nweeks{10}
       {
        try{
            for( int i=1; i<argc; ++i )
               {
                std::string_view arg{ argv[i] };
                if( arg.size()>=2 && arg[0]=='-' )
                   {// A command switch!
                    arg.remove_prefix(arg[1]=='-' ? 2 : 1); // Skip hyphen(s)
                    if( arg=="start"sv )
                       {// Date expected (like "2022-05-16")
                        if( ++i>=argc )
                           {
                            throw std::invalid_argument( std::format("Missing value after {}", arg) );
                           }
                        const auto maybe_date = conv::to_date( argv[i], date_fmt );
                        if( maybe_date )
                           {
                            m_startdate = *maybe_date;
                           }
                        else
                           {
                            throw std::invalid_argument(std::format("Invalid start date: {}", maybe_date.error()));
                           }
                       }
                    else if( arg=="weeks"sv )
                       {// Integer expected
                        if( ++i>=argc )
                           {
                            throw std::invalid_argument( std::format("Missing value after {}", arg) );
                           }
                        const auto maybe_weeks = conv::to_num<int>( argv[i] );
                        if( maybe_weeks )
                           {
                            m_nweeks = *maybe_weeks;
                           }
                        else
                           {
                            throw std::invalid_argument(std::format("Invalid weeks: {}", maybe_weeks.error()));
                           }
                       }
                    else
                       {
                        throw std::invalid_argument(std::format("Unknown switch: {}", arg));
                       }
                   }
                else
                   {
                    throw std::invalid_argument(std::format("Don't know how to use: {}", arg));
                   }
               }
           }
        catch( std::exception& e)
           {
            throw std::invalid_argument(e.what());
           }
       }

    [[nodiscard]] static auto curr_date() noexcept
       {
        return std::chrono::floor<std::chrono::days>(std::chrono::time_point<std::chrono::system_clock>{std::chrono::system_clock::now()});
       }
};

}//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
