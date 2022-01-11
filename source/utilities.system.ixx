module;

#if defined(_WIN32) || defined(_WIN64)
  #define MS_WINDOWS 1
#else
  #undef MS_WINDOWS
#endif

#ifdef MS_WINDOWS
  #include <Windows.h>
#else
  #include <sys/stat.h> // fstat
#endif

#include <string>
#include <stdexcept> // std::runtime_error
#include <cstring> // std::memcpy

export module utilities.system;

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace sys //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
{


/////////////////////////////////////////////////////////////////////////////
// Wrapper to system clipboard
export class Clipboard
{
 public:
    explicit Clipboard()
       {
      #ifdef MS_WINDOWS
        if(!::OpenClipboard(NULL)) throw std::runtime_error("Cannot access clipboard");
      #else
        <implement>
      #endif
       }

    ~Clipboard() noexcept
       {
      #ifdef MS_WINDOWS
        ::CloseClipboard();
      #else
        <implement>
      #endif
       }

    void set(const std::string& s) const
       {
      #ifdef MS_WINDOWS
        ::EmptyClipboard();
        const std::size_t len = s.size() + 1;
        HGLOBAL hMem = ::GlobalAlloc(GMEM_MOVEABLE, len); // | GMEM_DDESHARE
        std::memcpy(GlobalLock(hMem), s.c_str(), len);
        ::GlobalUnlock(hMem);
        ::SetClipboardData(CF_TEXT, hMem);
      #else
        <implement>
      #endif
       }

    std::string get() const noexcept
       {
        std::string text;
      #ifdef MS_WINDOWS
        // Get handle of clipboard object for ANSI text
        HANDLE hData = ::GetClipboardData(CF_TEXT);
        if(hData)
           {
            char* ptxt = static_cast<char*>( ::GlobalLock(hData) );
            if(ptxt) text = std::string(ptxt);
            ::GlobalUnlock( hData );
           }
      #else
        <implement>
      #endif
        return text;
       }
};


}//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
