/*  ---------------------------------------------
    calcreate
    ©2022 matteo.gattanini@gmail.com

    OVERVIEW
    ---------------------------------------------
    Utility to create calendar entries for my
    manual log files

    DEPENDENCIES:
    --------------------------------------------- */
#include <iostream>
#include <sstream> // std::istringstream
#include <string>
#include <string_view>
#include <format>
#include <chrono> // std::chrono::*
#include <stdexcept> // std::runtime_error
import utilities.string; // str::to_int
import utilities.system; // sys::*

using namespace std::literals; // "std::string literal"s
//using namespace std::chrono_literals; // 1d


/////////////////////////////////////////////////////////////////////////////
class Arguments
{
 public:
    Arguments(int argc, const char* argv[])
       {
        // Default start date
        const std::chrono::time_point<std::chrono::system_clock> today{std::chrono::system_clock::now()};
        i_startdate = std::chrono::floor<std::chrono::days>(today);

        //std::vector<std::string> args(argv+1, argv+argc); for( std::string& arg : args )
        try{
            enum class STS
               {
                SEE_ARG,
                GET_START,
                GET_WEEKS
               } status = STS::SEE_ARG;

            for( int i=1; i<argc; ++i )
               {
                const std::string_view arg{ argv[i] };
                switch( status )
                   {
                    case STS::SEE_ARG :
                        if( arg[0] == '-' )
                           {// A command switch!
                            if( arg=="-verbose"sv )
                               {
                                i_verbose = true;
                               }
                            else if( arg=="-start"sv )
                               {
                                status = STS::GET_START; // Date expected
                               }
                            else if( arg=="-weeks"sv )
                               {
                                status = STS::GET_WEEKS; // Integer expected
                               }
                            else
                               {
                                throw std::invalid_argument(std::format("Unknown switch: {}",arg));
                               }
                           }
                        else
                           {
                            throw std::invalid_argument(std::format("Don't know how to use: {}",arg));
                           }
                        break;

                    case STS::GET_START :
                       {
                        // Unfortunately 'parse' needs a stream, and it can't be constructed with a 'string_view'
                        const std::string sarg(arg);
                        std::istringstream iss(sarg);
                        //                            Notice that ↓, should be a ""sv
                        if( !(iss >> std::chrono::parse("%Y-%m-%d"s, i_startdate)) ) // Expecting something like "2022-05-16"
                           {
                            throw std::runtime_error(std::format("\"{}\" is not a valid date",arg));
                           }
                        status = STS::SEE_ARG;
                       } break;

                    case STS::GET_WEEKS :
                        i_nweeks = str::to_int(arg); // Expecting an integer
                        status = STS::SEE_ARG;
                        break;
                   }
               } // each argument
           }
        catch( std::exception& e)
           {
            throw std::invalid_argument(e.what());
           }
       }

    static void print_usage() noexcept
       {
        std::cerr << "\nUsage:\n"
                     "   calcreate -start 2022-05-16 -weeks 30\n"
                     "       -start <date>: Sets starting day\n"
                     "       -weeks <n>: Set how many weeks to generate\n"
                     "       -verbose: Log more info\n";
       }

    const auto& start_date() const noexcept { return i_startdate; }
    int weeks() const noexcept { return i_nweeks; }
    bool verbose() const noexcept { return i_verbose; }

 private:
    std::chrono::year_month_day i_startdate;
    int i_nweeks = 4;
    bool i_verbose = false;
    //fs::path i_output = ".";
};



//---------------------------------------------------------------------------
int main( int argc, const char* argv[] )
{
    std::ios_base::sync_with_stdio(false);

    try{
        Arguments args(argc, argv);

        if( args.verbose() )
           {
            std::cout << "**** calcreate (" << __DATE__ << ") ****\n"; // sys::human_readable_time_stamp()
            //std::cout << "Running in: " << fs::current_path().string() << '\n';
            std::cout << args.weeks() << "   weeks from " << args.start_date() << '\n' << '\n';
           }

        std::ostringstream sout;
        std::chrono::sys_days curr_days(args.start_date());
        //std::chrono::time_point<std::chrono::system_clock, std::chrono::days> curr_days(args.start_date());
        int remaining_weeks = args.weeks();
        while( remaining_weeks>0 )
           {
            //std::cout << "  " << std::format("{:%Y-%m-%d}", curr_days) << "  " << std::format("{:%A}", std::chrono::weekday{curr_days}) << '\n';
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
                sout << '[' << std::format("{:%Y-%m-%d}", curr_days);

                     if( weekday==std::chrono::Monday ) sout << " mon" "]\n";
                else if( weekday==std::chrono::Friday ) sout <<        "]\n" "\n";
                else                                    sout <<        "]\n";

                ++curr_days; // curr_days += std::chrono::days{1};
               }
           }

        const std::string s = sout.str();
        std::cout << s;

        sys::Clipboard clipboard;
        clipboard.set( s );

        return 0;
       }
    catch(std::invalid_argument& e)
       {
        std::cerr << "!! " << e.what() << '\n';
        Arguments::print_usage();
       }
    catch(std::exception& e)
       {
        std::cerr << "!! Error: " << e.what() << '\n';
       }
    return -1;
}
