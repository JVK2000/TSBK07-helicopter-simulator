### Questions for lab 3

#### Part 1)

**How can you get all four blades to rotate with just one time-dependent rotation matrix?**

You can define a time-dependent rotation matrix and multiply it with a constant rotation matrix defined for every blade
```C
mat4 time_rot = Rx(t/1000)

// Do this for all blades
mat4 tot_rot = Mult(time_rot, Rx(M_PI/2)) 
```


<br>

**How do you make the wings follow the body's movements?**

After all transformations (related to all models' positioning and the rotation of the blades), you can transform the complete set of models with a transformation. 


---

#### Part 2)

**What kind of control did you implement?**
Movement: **w** - forward, **s** - backward, **a** - left, **d** - right. 
Looking: **mouse** - up, down, left, right

**Can you make this kind of control in some other way than manipulating a "look-at" matrix?**
By creating an extra matrix for movement and one for rotation of the projectionMatrix


----

#### Part 3)

**How did you handle the camera matrix for the skybox?**

I created a rotation matrix based on the camera angles I used when creating the camera matrix. In this way, I *zeroed out the translation part*.

</br>

**How did you represent the objects? Is this a good way to manage a scene, or would you do it differently for a "real" application?**

I'am drawing each model directly to the world. You can use **Scene Graph** to organize models in a hierarchy for bigger projects.  

See: https://learnopengl.com/Guest-Articles/2021/Scene/Scene-Graph

</br>

**What special considerations are needed when rendering a skybox?**

* No Z-buffering. 
* Skybox should have the same rotation as the camera but not move. 


</br>

**What is the problem with the “skybox" object used in the lab? (The problem doesn't have to be corrected.)**

There is a thin line in the corners of the skybox.


----

#### Part 4)

**Why was blue facing the camera when visualizing the normal vectors?**

When a surface faces a camera, the normal becomes (0, 0, 1), and when using the normal to set the color, the color becomes blue because of RGB.


</br>

**For light source 3, why did we get a white area in the distance for the specular light but not for the diffuse?**

Specular light uses the viewing angle, which diffuse light doesn't use.

(When using specular light, the intensity of the light is highest when looking at the light source.)

<!-- "The intensity of the specular reflection is highest when the viewer looks directly at the light source and decreases as the viewing angle deviates from this direction. This can create bright spots or white areas in the distance, especially when the light source is far away and the viewer is looking at the scene from a particular angle."

So, you may have observed a white area in the distance for specular light but not for diffuse light because the Phong lighting model considers the viewing angle, while the Lambertian model does not. -->


</br>

**How do you generate a vector from the surface to the eye?**

`cameraPosition - surfacePosition`



</br>

**Which vectors need renormalization in the fragment shader?**

- `lightSourcesDirPosArr`
- `normal`
- `cameraPosition - surfacePosition`




