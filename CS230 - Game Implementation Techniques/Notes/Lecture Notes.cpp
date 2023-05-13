Lecture 1:

if there is high fps (frame per sec), then all events will look like running 
simulatanteously. 

Events:
Physics, input, logic, collision, audio, graphics

Why interactive?

Players decide:
- When & where to move the ship,


dynamic objects

What is Real Time: 
The programmer is building every single frame of the simulation from scratch

60fps means is refreshing 60 times per secs. 1/60 = 0.016s

To decide the colour of the pixel, 

minimum to be 120fps for the most smooth experience, fps need to be machine
tested. if playing tetris then dont need to be so high fps. 

the higher the fps, the smoother the visualibility.

frame base gameloop vs timebase gameloop

if i rely on time on my computation, the bullet will reach same position
regardless of fps

if i rely on fps, then 10fps, the computation will only run 10 times while 60fps
will run 60 times and the bullet will run further. 


1. input (use mouse, move camera, press key)
2. logic (is space key is pressed? is mouse pressed?)
3. physics (movements)
4. collision 
5. audio
6. graphics (related to VGA, VGA is displaying all the pixels

Lecture 2:

if(keyboard.IsPressed(SPACE))
{
  mainCharacter.JUMP();
  mainCharacter.ADDFORCE(vect(0,1),MAG);
}

set properties, set attributes, 

physics - 

Game Loop (1/60)
{
  //start time
  
  INPUT.UPDATE
  LOGIC
  PHYSICS
  COLLISION
  AUDIO
  GRAPHICS
  
  //end time
}

delta time = start - end time; 


