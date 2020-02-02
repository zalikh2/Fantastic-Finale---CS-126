### 11-16, 11-17
I spent this weekend starting my build environment. I started setting up the GUIs for the shapes as well as for the settings.

### 11-18
I was able to create a function to interpolate between two colors, which I will use later when I fully integrate sound. 
I also started to write my tests, but i found out one of my external libraries was untestable.

### 11-19
I was able to let the user customize the input for a time intervl and shapes to draw. Based on the input the program would 
draw different shapes at different times. This wasn't completely the goal of my visualizer but testing this out will help me
when implementing the final visulaizer. I also added more documentation and testing.

### 11-25
I changed a lot of the Shape drawing by making it an enum with static methods. I also created a ParticleSettings struct
which is going to hold all the settings for a given particle and will change whenever the GUI is interacted with. Ialso 
started to set up how the sound numbners will be calculated so that I can start the visualizations.

### 11-26 to 12-1
This was during the majority of break so I wasn't able to get much done. However, I was able to get a rudimentary version of
the visualizer working. I found that the beat numbers were super low, so I might have to scale up the numbers to actually create a
decent visualization.

### 12-2
I created new features for the visualization such as the particles switching between colors. I also rearranged some of the functions
in the classes in order to avoid circular dependencies and make the function placement more sensible.

### 12-6
I created new features like smoothing and scaling that will help the visualizer look better. I also implement drawing multiple particles
centered around a certain point, and the user can determine how far those points are from the center. I still need to add more song 
options though.

### 12-7 to 12-8
I added fixes to drawing multiple points around a given center. I also added the ability to reset the particle settings and GUIS. I also
added more song options for the user to choose from. 

### 12-9 
I added some more bug fixes, refactored some code, and added more documentation.

### 12-11
I added the last touches like more documentation and added a feature for relative scaling, which makes the particles show up
better, as well as extracting some magic numebrs into constants. I also created the README for instructions on how to 
use the program on one's PC.