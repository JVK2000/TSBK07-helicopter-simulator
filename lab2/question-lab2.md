### Questions for lab 2

#### Part 1)

**How are the textures coordinates mapped on the bunny? Can you see how they vary over the model?**

The texture coordinates are mapped with *Linear mapping*

when texture coordinates is (s, t) = (0, 0) the color will be blue. RGB - (s, t, 1) and when the we have (s, t) = (1, 1) we will have white.

**What kind of procedural texture did you make?**

Interpolated texture coordinates as color.



#### Part 2)

**How can you make a texture repeat multiple times over the bunny?**

With GL_TEXTURE_WRAP_S and GL_TEXTURE_WRAP_T 


**Why can't we just pass the texture object to the shader? There is a specific reason for this, a limited resource. What? (No, it is not that we must avoid re-uploading from CPU. The texture object is on the GPU!)**



*- In order to use pre-generated texture data, they must be loaded into a texture object by the host program, which must then be attached to a texture unit, and then you should communicate the texture unit number to the shader.*

*- Texture units are hardware resources for looking up textures*

*- The shader uses the texture unit ID, not the texture object*

Texture units are used because there are fast. We have 16 texture units


#### Part 3)

**How did you move the bunny to get it in view?**

I used a transform with position (0, 0, -2).


#### Part 4)

**Given a certain vector for v, is there some place you can't place the camera?**

you cant give l (up-vector) the value (0, 0, 0). In that case, no axis is up, and therefore, nothing is shown.  