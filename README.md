# CGTP
Computer Graphics Pratical Assignment, University of Minho 2016/2017

## Requirements
Install cmake;
OpenGL - Glut and the extension glew;
DevIL

# For linux you only need to have them installed:
(apt package manager example)
	– sudo apt-get install cmake
	– sudo apt-get install cmake-qt-gui
• Install freeglut
	– sudo apt-get install freeglut3-dev
• Check OpenGL version
	– glxinfo | grep "OpenGL“
	– (sudo apt-get install mesa-utils)

– sudo apt-get install libxmu-dev libxi-dev
• Install GLEW
	– sudo apt-get install libglew-dev

• Install Linux
	– sudo apt-get install libdevil-dev

# For windows you need to have to install cmake from the website and have the files organized as follows:
	toolkits
		devil
			IL
				il.h
			devil.dll
			DevIL.lib
		glew
			GL
				glew.h
			glew.zip
			glew32.dll
			glew32.lib
		glut
			GL
				glut.h
			glut32.dll
			glut32.lib


## Run
Use cmake:  
source directory: src;  
build directory: build;  

./generator(or generator.exe or VS w/ cmd args) \<shapename\> \<shapeArgs\> \<fileForVertexes\>  

./engine(or engine.exe or VS w/ cmd args) \<xmlFile\> (optional, go to note)

### Shapes available on generator and their arguments:  

	plane xLength(float or int) zLength(float or int) divisions filename
    box xLength y(height) zLength divisions(of each face) filename
    sphere radius slices stacks filename
    cone radius height slices stacks filename
    cylinder bottomRadius topRadius height slices stacks filename
    torus innerRadius outterRadius sides rings filename
    ruby middleRadius topRadius bottomHeight TopHeight slices stacks filename
    bezier inputPatches Tesselation
    crown innerRadius outterRadius Sides


Example scenes for each shape available(solar_system.xml, creeper.xml, sphere.xml, etc...)
Load them on engine to see the examples.

### Note:  
If you use the generator to create new shape,
don't forget to add the file name at config.xml,
    or other xml file using that form (in this case you must
    give the path as an argument for engine)

## FPS Cam 
Use Arrow keys to change direction;
Use WASD to move;

## Change fill/line/point
F1;
