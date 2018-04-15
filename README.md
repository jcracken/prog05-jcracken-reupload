## Notes

I've gotten the code to a point where it renders everything, but there is weird artifacting all over the sphere. ~~First, in the form of weird gray shapes being rendered over the sphere. This might be related to my shading algorithm, which doesn't seem to apply the specular effect properly. It's a shame, because as you flip between the three shading types, you can make out under the artifacts that it seems to somewhat work? You can see the triangle edges disappear going from flat to gourand, and phong seems to be even smoother.~~ The professor pointed out two bugs related to my z-buffer that were responsible for them, and fixing those bugs seemed to resolve that issue. The second source of artifacting are weird symmetric black lines on the shape. They just randomly appeared after I fixed a bug related to triangle normal calculation. I have zero idea what caused them.

The startup time is roughly 15 seconds for one sphere and two spheres. This is down from about a minute before, and even longer before I optimized the code a bit. The main bottleneck is the code from assignment 4. The inefficiencies there also prevented me from testing the ballring or kitten testcases.

Additionally, I wasn't able to get the code to render a second sphere. I found out that it only renders one shape after many, many hours of trying to get the shading to work properly, so I don't have it in me to see what the problem is. Like the black lines, this problem appears across all three types of shading, and I can't imagine why. My guess is it might have to do with my shading algorithm? I make sure to make as many sets of data as there are shapes, so (shrug).

But, as far as I can tell, the parsing works perfectly, the transformation matrices are calculated correctly, the vertices are transformed correctly, I have a working z-buffer that only keeps the closest fragment, my barycentric coordinates are calculated properly, I let the user switch between all three types of shading, and by the submission time I will have included a custom scene.

The only issues I can find are: for phong shading, it often renders the lights as being in the wrong position (likely, because the shading values are calculated after vertex transformation, causing some sort of issue with the position of the shapes and lights) and specular shading may not work, or may just be on the other side of the sphere and it can't be seen; the weird artifacting with the black lines; and the second shape doesn't get rendered.

## References

This directory includes both some models in Wavefront .obj format as well as some scene files to test the rasterizer

Models in Wavefront .obj format

* sphere{1,2}.obj are two different spheres, both of radius 1.   sphere2.obj is translated 3 units in the x direction.
* torus4.obj is a new torus
* kitten.obj is the well known kitten model by Frank B. ter Haar, first appearing in "Frank B. ter Haar, Remco C. Veltkamp:
Automatic multiview quadruple alignment of unordered range scans. Shape Modeling International 2007: 137-146" and available from [http://visionair.ge.imati.cnr.it/ontologies/shapes/view.jsp?id=270-Kitten_-_merged](http://visionair.ge.imati.cnr.it/ontologies/shapes/view.jsp?id=270-Kitten_-_merged).


Scenes included are:

* 1sphere.txt is a single, red sphere
* 2spheres{1,2,3}.txt are three scenes rendering both spheres.  The second sphere is rendered blue.  These scenes test that z-buffering is working correctly by positioning the eye in different locations
* ballring.txt is a scene that includes more complex lighting and has both the sphere and torus
* kitten.txt is a scene to test the larger triangle mesh with more complex lighting.

Other References

https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/barycentric-coordinates 
https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage
for barycentric stuff