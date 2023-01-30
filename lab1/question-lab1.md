### Questions for lab 1

#### Part 1)

**Where is the origin placed in the on-screen coordinate system?**
The origin is placed in the middle of the screen. 

**Which direction are the X and Y axes pointing in the on-screen coordinate system?**
X-axis: Left to right
Y-axis: Bottom to top 

**The triangle color is controlled from the fragment shader. Would it be possible to control it from the main program? How?**
You can use the uniform modifier if a single color is going to be used by the model. 



#### Part 2)

**What is the purpose of the "in", "out" and "uniform" modifiers?**
Data can be passed from shader to shader with `in` and `out` modifiers.

`in`: Takes a read-only variable, which represents a vector. This vector consists of data that are used by one pixel (takes a section of the buffer).

`out`: Output only variable. Here we are gathering a vector. 

`uniform`: Constant variable. It is the same for every execution of shader during the rendering of a primitive. (*source: https://www.khronos.org/opengl/wiki/Type_Qualifier_(GLSL)#Uniforms*)


**What is the output of the fragment shader?**
Fragment shader calculates lighting effects and texturing. In this lab, it outputs color values. 


**What does the function glUniformMatrix4fv do?**
Pass a matrix to a shader program.



#### Part 3

**What is the frame rate of the animation?**
When using ```glutRepeatingTimerFunc(20);```, we update every 20 milliseconds, meaning 50 fps. This is the maximum number of frames.



#### Part 4

**Did you need to do anything different when uploading the color data?**
I first needed to load the data to the vertex shader to be able to "pass" it to the fragment shader. 

This is because is can only create (```glGenVertexArrays()```) and activate (```glBindVertexArray()```) one container per model ????


**The "in" and "out" modifiers are now used for something different. What?**
Pass variables from vertex to fragment shader. 



#### Part 5

**What problems did you encounter while building the pyramid?**
That the orientation of the vertices mattered, that there should be either clockwise or counterclockwise. 


**How do you change the facing of a polygon**
you can use ```glCullFace(GL_FRONT)```. you can use ```glEnable(GL_CW)``` or ```glEnable(GL_CCW)```. 
 


#### Part 6

**Why do we need normal vectors for a model?**
They are used for calculating light. (*source: https://learnopengl.com/Lighting/Basic-lighting*)


**What did you do in your fragment shader?**
Base the model's color on the normal.


**Should a normal vector always be perpendicular to a certain triangle? If not, why?**
Yes, a normal is perpendicular to its surface (the triangle) at any given point.


**Now we are using glBindBuffer and glBufferData again. They deal with buffers, but in what way?**
They enable a GPU buffer, reference it, and load data to it.