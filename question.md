### Questions for lab 1

#### Part 1)

**Where is the origin placed in the on-screen coordinate system?**



**Which direction are the X and Y axes pointing in the on-screen coordinate system?**

**The triangle color is controlled from the fragment shader. Would it be possible to control it from the main program? How?**



#### Part 2)

**What is the purpose of the "in", "out" and "uniform" modifiers?**

**in**: The value of the parameter will be copies into the parameter when the function is called. The function may change the value of the parameter, but those chages will only be pressent inside the function.  

**out**: The value of the parameter will not be initialized by the caller. The parameters value is set inside the function, and after the functions execution is compleated, the value of the parameter will be copied into the variable that the user specifies when calling the function. (*source: https://gamedev.stackexchange.com/questions/29672/in-out-keywords-in-glsl*)

**uniform**: The variable declared with this modifier dose not change between multiple executions of shader during the rendering of a primitive. They are simply put constants. (*source: https://www.khronos.org/opengl/wiki/Type_Qualifier_(GLSL)#Uniforms*)


**What is the output of the fragment shader?**
*"The output of a fragment shader is a depth value, a possible stencil value (unmodified by the fragment shader), and zero or more color values to be potentially written to the buffers in the current framebuffers."* (*source: https://www.khronos.org/opengl/wiki/Fragment_Shader*)


**What does the function glUniformMatrix4fv do?**
Specify the value of a uniform variable for the current program object (*source: https://registry.khronos.org/OpenGL-Refpages/gl4/html/glUniform.xhtml*)


#### Part 3

**What is the frame rate of the animation?**
if we are using ```glutRepeatingTimerFunc(20);``` then we will update every 20 milisecunds. There will be 1000 / 20 = 50 updates per secund, aka 50 fps




