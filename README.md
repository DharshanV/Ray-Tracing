# Ray Tracing
A simple Ray Tracer with basic geometry, lighting, reflection, and cubemap. 

## What I learned
- Ray Intersections with sphere and plane. 
  - Vector operations: dot product and cross products.
  - Usefulness of normal vectors. Such as shadows and lightings
- Getting pixel color value of cubemap from normal vector direction
- Going form pixel coor (u,v) to view direction from camera (x,y,z)
- Multithreading
  - Breaking down screen into different parts for parallel ray tracing
## What can be improved
- Biggest improvment is to implement path tracing
- Putting scene data (triangles) into spatial data structure ([Octree](https://github.com/DharshanV/Octree-QuadTree))
- Dynamic multithreading i.e. easily define number of splits per rectangle

## All Rendering
![Output sample](https://github.com/DharshanV/Ray-Tracing/blob/master/Rendering%20Progress/All%20Rendering.png)
