#include "core/engine.h"

int main()
{
	cat::core::engine* e = cat::core::engine::get_instance();
	e->run();


	return 0;
}