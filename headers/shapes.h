#ifndef __SHAPES_H_INCLUDED__
#define __SHAPES_H_INCLUDED__

#include <vector>
#include <string>

/**
    Returns vertexes for a plane.
    
    @param x Length of the plane on X axis.
    @param y Position of the plane on Y axis.
    @param z Length of the plane on Z axis.
    @param div Number of divisions of the plane.
    @return Vector with all strings of the vertexes.
*/
std::vector<std::string> plane(float x, float y, float z, int div);

/**
    Returns vertexes for a box.
    
    @param x Length of the box on X axis.
    @param y Box's height.
    @param z Length of the box on Z axis.
    @param div Number of divisions of each face of the box.
    @return Vector with all strings of the vertexes.
*/
std::vector<std::string> box(float x, float y, float z, int div);

/**
    Returns vertexes for a sphere.

    @param radius Radius of the sphere.
    @param slices Number of slices.
    @param stacks Number of stacks.
    @return Vector with all strings of the vertexes.
*/
std::vector<std::string> sphere(float radius, int slices, int stacks);

/**
    Returns vertexes for a cylinder or cone(if top radius is 0).

    @param radB Bottom radius.
    @param radT Top radius.
    @param height Height of the cylinder/cone.
    @param slices Number of slices.
    @param stacks Number of stacks.
    @return Vector with all strings of the vertexes.
*/
std::vector<std::string> cylinder(float radB, float radT, float height, int slices, int stacks);

/**
    Returns vertexes for a torus.

    @param inner Inner radius.
    @param outer Outer radius.
    @param sides Number of sides.
    @param rings Number of rings.
    @return Vector with all strings of the vertexes.
*/
std::vector<std::string> torus(float inner, float outer, int sides, int rings);

#endif