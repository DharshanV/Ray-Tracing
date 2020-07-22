# Ray Tracing
A simple Ray Tracer with basic geometry, lighting, reflection, cubemap, and supersampling. 

I also wrote a book, that covers building a ray tracer from scratch. I walk through step by step on what all the code means and how they work. The book can be found be found above, or the link [here](https://drive.google.com/file/d/1Gst_P7AaeW43OLikB3RMqsf97JPNXFzq/view).

## How to run
The project comes with a tasks.json. So pressing Ctrl + Shift + b, should
compile the program and create a exectuable "renderMain".

Without using the tasks.json. We can compile using g++.

In the root folder, type the commands:
```console
mkdir build
cd build
cmake ..
make
```

This will also create a exectuable "render" inside the build folder.

To run the exectuable, expected arugments are:
```console
./build/render width height fileName OR

./build/render width height splitCount fileName
```

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

## All Rendering
![Output sample](https://i.imgur.com/fwfgw0l.png)
