#define GLM_FORCE_ALIGNED_GENTYPES
#include "Npgs.h"
#include "Application.h"

#include <exception>
#include <fstream>
#include <memory>
#include <windows.h>

using namespace Npgs;
using namespace Npgs::Util;

int main()
{
    // This marker uses the Win32 API directly, so it also works before the
    // logging library and the application runtime are initialized.
    if (HANDLE StartupMarker = CreateFileW(
            L"startup-stage.txt", GENERIC_WRITE, FILE_SHARE_READ, nullptr,
            CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        StartupMarker != INVALID_HANDLE_VALUE)
    {
        constexpr char Marker[] = "entered main\r\n";
        DWORD BytesWritten = 0;
        WriteFile(StartupMarker, Marker, sizeof(Marker) - 1, &BytesWritten, nullptr);
        CloseHandle(StartupMarker);
    }

    try
    {
        FLogger::Initialize();

        // Keep the application object off the relatively small Windows main
        // thread stack and make startup failures visible in packaged builds.
        auto App = std::make_unique<FApplication>(
            vk::Extent2D{ 1280, 960 }, "Learn glNext FPS:", false, true);
        App->ExecuteMainRender();
        return 0;
    }
    catch (const std::exception& Error)
    {
        std::ofstream("startup-error.txt", std::ios::trunc)
            << "Unhandled startup exception: " << Error.what() << '\n';
        return 1;
    }
    catch (...)
    {
        std::ofstream("startup-error.txt", std::ios::trunc)
            << "Unhandled non-standard exception during startup.\n";
        return 2;
    }
}
