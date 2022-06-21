#include "Core.h"

int main(int argc, char* argv[])
{
	Core::Initialize("Shooter", 1280, 720);
	Core::Run();
	Core::Shutdown();

	return 0;
}
