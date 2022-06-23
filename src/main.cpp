#include "Core.h"

int main(int argc, char* argv[])
{
	Core::Initialize("Atmosphere Rendering", 1600, 900);
	Core::Run();
	Core::Shutdown();

	return 0;
}
