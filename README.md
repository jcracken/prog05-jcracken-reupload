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
