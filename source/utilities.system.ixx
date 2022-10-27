module;
//  ---------------------------------------------
//  Clipboard manipulation
//  ---------------------------------------------
#if defined(_WIN32) || defined(_WIN64)
  #define MS_WINDOWS 1
#else
  #undef MS_WINDOWS
#endif

#if defined(MS_WINDOWS)
  #include <Windows.h>
#endif

#include <stdexcept> // std::runtime_error
#include <string>
#include <string_view>
#include <cstring> // std::memcpy

export module utilities.system;


//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
namespace sys //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
{

  #if defined(MS_WINDOWS)
    /////////////////////////////////////////////////////////////////////////
    // Wrapper to GlobalLock/GlobalUnlock
    class WinGlobalMemLock final
    {
     public:
        WinGlobalMemLock(HGLOBAL h) noexcept
           : m_hmem(h)
           , m_ptr(::GlobalLock(m_hmem)) {}

        ~WinGlobalMemLock() noexcept
           {
            ::GlobalUnlock(m_hmem);
           }

        [[nodiscard]] HGLOBAL handle() const noexcept { return m_hmem; }
        [[nodiscard]] LPVOID data() const noexcept { return m_ptr; }

     private:
        HGLOBAL m_hmem;
        LPVOID m_ptr;
    };
  #endif



/////////////////////////////////////////////////////////////////////////////
// Wrapper to system clipboard
export class Clipboard final
{
 public:
    Clipboard()
       {
      #if defined(MS_WINDOWS)
        if( !::OpenClipboard(NULL) )
           {
            throw std::runtime_error("Cannot access clipboard");
           }
      #elif defined(POSIX)
        <implement>
      #endif
       }

    ~Clipboard() noexcept
       {
      #if defined(MS_WINDOWS)
        ::CloseClipboard();
      #elif defined(POSIX)
        <implement>
      #endif
       }

    void set(const std::string_view sv) const noexcept
       {
      #if defined(MS_WINDOWS)
        ::EmptyClipboard();
        WinGlobalMemLock mem{ ::GlobalAlloc(GMEM_MOVEABLE, sv.size()+1) }; // | GMEM_DDESHARE
        if( mem.data() )
           {
            char* const txt_arr = static_cast<char*>(mem.data());
            std::memcpy(txt_arr, sv.data(), sv.size());
            txt_arr[sv.size()] = '\0'; 
            ::SetClipboardData(CF_TEXT, mem.handle());
           }
      #elif defined(POSIX)
        <implement>
      #endif
       }

    [[nodiscard]] std::string get() const noexcept
       {
        std::string text;
      #if defined(MS_WINDOWS)
        // Get handle of clipboard object for ANSI text
        if( HANDLE hData = ::GetClipboardData(CF_TEXT) )
           {
            WinGlobalMemLock mem(hData);
            if( mem.data() )
               {
                const char* const txt_arr = static_cast<char*>(mem.data());
                text = std::string(txt_arr);
               }
           }
      #elif defined(POSIX)
        <implement>
      #endif
        return text;
       }
};


}//::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
