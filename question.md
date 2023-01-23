### Questions for lab 1

#### Part 1)

**Where is the origin placed in the on-screen coordinate system?**
The origin is placed in the middle of the screen. 

**Which direction are the X and Y axes pointing in the on-screen coordinate system?**
The x-axis is horizontal, while the y-axis is vertical.

**The triangle color is controlled from the fragment shader. Would it be possible to control it from the main program? How?**
You can use the uniform modifier if a single color is going to be used by the model. 



#### Part 2)

**What is the purpose of the "in", "out" and "uniform" modifiers?**

**in**: The parameter's value will be copied into the parameter when the function is called. The function may change the parameter's value, but those changes will only be present inside the function. 

**out**: The parameter's value will not be initialized by the caller. The parameter value is set inside the function. After the function execution is completed, the parameter's value is copied into the variable that the user specifies when calling the function. (*source: https://gamedev.stackexchange.com/questions/29672/in-out-keywords-in-glsl*)

**uniform**: The variable declared with this modifier does not change between multiple executions of shader during the rendering of a primitive. They are, simply put, constants. (*source: https://www.khronos.org/opengl/wiki/Type_Qualifier_(GLSL)#Uniforms*)


**What is the output of the fragment shader?**
*"The output of a fragment shader is a depth value, a possible stencil value (unmodified by the fragment shader), and zero or more color values to be potentially written to the buffers in the current framebuffers."* (*source: https://www.khronos.org/opengl/wiki/Fragment_Shader*)


**What does the function glUniformMatrix4fv do?**
Specify the value of a uniform variable for the current program object (*source: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUniform.xhtml*)


#### Part 3

**What is the frame rate of the animation?**
if we are using ```glutRepeatingTimerFunc(20);``` then we will update every 20 milisecunds. There will be 1000 / 20 = 50 updates per second, aka 50 fps



#### Part 4

**Did you need to do anything different when uploading the color data?**
I first needed to load the data to the vertex shader to be able to "pass" it to the fragment shader. 

This is becouse is can only create (```glGenVertexArrays()```) and activate (```glBindVertexArray()```) one container per model ????


**The "in" and "out" modifiers are now used for something different. What?**
They are partly used for passing data from one buffer to another. 



#### Part 5

**What problems did you encounter while building the pyramid?**
That the orientation of the vertices mattered, that there should be either clockwise or counterclockwise. 


**How do you change the facing of a polygon**
you can use ```glCullFace(GL_FRONT)```. you can use ```GL_CW()``` or ```GL_CCW()```. 
 


#### Part 6

**Why do we need normal vectors for a model?**
They are used for calculating light. (*source: https://learnopengl.com/Lighting/Basic-lighting*)


**What did you do in your fragment shader?**
Set the color to the nose and eyes for the model.


**Should a normal vector always be perpendicular to a certain triangle? If not, why?**
Yes, a normal is perpendicular to its surface (the triangle) at any given point.


**Now we are using glBindBuffer and glBufferData again. They deal with buffers, but in what way?**
**glBindBuffer** binds a buffer object to a target. (*source: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glBindBuffer.xhtml*)
**glBufferData** creates a data store for the buffer object bound to the specified target. (*source: https://docs.gl/gl3/glBufferData*)