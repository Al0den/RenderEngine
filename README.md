# Render Engine

This is an all purpose render engine, built only using SDL2 (And SDL\_ttf). 
It contains some basic features, all extensible and overridable.

By default, this renderer already runs in a separate thread.

# How to use

This requires the use of the C++14 standard (For the use of vectors)

Simply clone the reposity, and run `make -B`. Then, in any project, include the `include/RenderEngine"` file. When linking, you will need to link the library built inside `lib`, SDL2 and SDL\_ttf

Another option is to recompile the whole code, and simply include it (Still linking SDL2 and SDL\_ttf though) 

# Screenshots

WIP

