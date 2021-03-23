#include <iostream>
#include "engine/engine.h"
#include "render/renderer.h"

int main()
{
	std::shared_ptr<Engine> engine = std::make_shared<Engine>();
	std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>();

	if (!engine->startup())
		return 1;
	
	engine->run();
	engine->shutdown();
	
    return 0;
}