### Questions for lab 1

#### Part 1)

**Where is the origin placed in the on-screen coordinate system?**



**Which direction are the X and Y axes pointing in the on-screen coordinate system?**

**The triangle color is controlled from the fragment shader. Would it be possible to control it from the main program? How?**



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
I first needed to load the data to the vertex shader to be able to "pass" it to the fragment shader. this is becouse is can only create (```glGenVertexArrays()```) and activate (```glBindVertexArray()```) one container per model ????


**The "in" and "out" modifiers are now used for something different. What?**
They are partly used for passing data from one buffer to another. 

