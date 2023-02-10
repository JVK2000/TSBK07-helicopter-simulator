### Questions for lab 3

#### Part 1)

**How can you get all four blades to rotate with just one time-dependent rotation matrix?**

You can define a time-dependent rotation matrix and multiply it with an constant rotation matrix defined for every blade
```C
mat4 time_rot = Rx(t/1000)

// Do this for all blades
mat4 tot_rot = Mult(time_rot, Rx(M_PI/2)) 
```

I decided to this instead
```C
// Do this for all blades
mat4 tot_rot = Rx(M_PI/2 + t/1000) 
```

</br>

**How do you make the wings follow the body's movements?**

After all transformations are done (related to all models positioning and the blades rotation) you can transform the compleat set of models with an transformation. 


</br>

#### Part 2)
