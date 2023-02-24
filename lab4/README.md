### Questions for lab 4

#### Part 1)

**What kind of data does the heightmap image data hold? What range is the heightmap data?**

Each value in the heightmap represents the height of the point on the surface. 

Value from 0 to 1, -1 to 1 or 0 to 255



</br>

**The terrain is 4x4. How many polygons does it draw?**

```math
(width-1) * (height-1) = 3 * 3 = 9
```


----

#### Part 2)

**Did you need to extend your camera movement code? How?**

Add added the ability to move upp and down (along the y-axis)

Movement: **w** - forward, **s** - backward, **a** - left, **d** - right, **q** - down, **e** - up. 
Looking: **mouse** - up, down, left, right