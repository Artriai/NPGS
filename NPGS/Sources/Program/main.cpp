#define GLM_FORCE_ALIGNED_GENTYPES
#include "Npgs.h"
#include "Application.h"

#include <exception>
#include <fstream>
#include <memory>

using namespace Npgs;
using namespace Npgs::Util;

int main()
{
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
