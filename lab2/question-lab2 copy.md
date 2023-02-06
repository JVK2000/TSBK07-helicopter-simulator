### Questions for lab 2

#### Part 1)

**How are the textures coordinates mapped on the bunny? Can you see how they vary over the model?**

<!--*For every vertex in the model, project it onto the surface. This point can then be transformed to a coordinate system from which we extract the texture coordinates (s,t).*
 *Each vertex has a texture coordinate; interpolate between, look up texture with interpolated coordinates.*-->

 The texture coordinates are mapped with *Linear mapping*
 

**What kind of procedural texture did you make?**

 Interpolated texture coordinates as color



#### Part 2)

**How can you make a texture repeat multiple times over the bunny?**

With GL_TEXTURE_WRAP_S and GL_TEXTURE_WRAP_T 


**Why can't we just pass the texture object to the shader? There is a specific reason for this, a limited resource. What? (No, it is not that we must avoid re-uploading from CPU. The texture object is on the GPU!)**



*- In order to use pre-generated texture data, they must be loaded into a texture object by the host program, which must then be attached to a texture unit, and then you should communicate the texture unit number to the shader.*

