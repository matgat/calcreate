//import std;
#include <format>
#include <print>
#include <chrono>
#include <stdexcept>

import app.arguments; // app::Arguments
import utilities.clipboard; // sys::Clipboard

#define DATEFMT "%Y-%m-%d"

//---------------------------------------------------------------------------
int main( int argc, const char* argv[] )
{
    try{
        app::Arguments args(argc, argv, DATEFMT);

        std::ostringstream sout;
        std::chrono::sys_days curr_days(args.start_date());
        int remaining_weeks = args.weeks();
        while( remaining_weeks>0 )
           {
            const std::chrono::weekday weekday{curr_days};
            if( weekday==std::chrono::Sunday )
               {
                curr_days += std::chrono::days{1};
                --remaining_weeks;
               }
            else if( weekday==std::chrono::Saturday )
               {
                curr_days += std::chrono::days{2};
                --remaining_weeks;
               }
            else
               {
                sout << std::format("[{:" DATEFMT "}", curr_days);

                     if( weekday==std::chrono::Monday ) sout << " mon" "]\n";
                else if( weekday==std::chrono::Friday ) sout <<        "]\n\n";
                else                                    sout <<        "]\n";

                ++curr_days;
               }
           }

        const auto s{ sout.str() }; // Should be move constructed
        std::print("{}",s);

        sys::Clipboard clipboard;
        clipboard.set( s );

        return 0;
       }
    catch(std::invalid_argument& e)
       {
        std::print("!! {}", e.what());
        std::print( "\nUsage:\n"
                    "   calcreate -start 2022-05-16 -weeks 30\n"
                    "       -start <" DATEFMT ">: Sets starting day\n"
                    "       -weeks <n>: Set how many weeks to generate\n" );
       }
    catch(std::exception& e)
       {
        std::print("!! Error: {}", e.what());
       }
    return 2;
}
