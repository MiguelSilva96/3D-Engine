# CGTP
Computer Graphics Pratical Assignment, University of Minho 2016/2017

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


Example scenes for each shape available(box.xml, sphere.xml, etc...),  
Load them on engine to see the examples.

### Note:  
If you use the generator to create new shape,
don't forget to add the file name at config.xml,
    or other xml file using that form (in this case you must
    give the path as an argument for engine)

## Explorer Cam 
Use Arrow keys;

## Change fill/line/point
F1;
