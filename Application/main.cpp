#include "core/engine.h"

int main()
{
	cat::core::engine* e = cat::core::engine::getInstance();
	e->run();


	return 0;
}