#include "pch.h"
#include "Engine.h"


int main()
{
    std::srand(static_cast<unsigned int>(std::time(NULL)));

    Engine engine;

    engine.run();

    return 0;
}