sdl_basic_2d
============

basic 2d application using SDL 2.0

0) Prerequisites(debian-like):
    
    sudo apt-get install libsdl2-2.0-0 libsdl2-dev libsdl2-image-2.0-0 libsdl2-image-dev
    sudo ln -s /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0 /usr/lib/libSDL.so
	sudo ln -s /usr/lib/x86_64-linux-gnu/libSDL2_image-2.0.so.0.0.0 /usr/lib/libSDL_image.so
    
1) Building project
    
    cmake .
    make
    
2) Running project
    
    ./build/x_proj
