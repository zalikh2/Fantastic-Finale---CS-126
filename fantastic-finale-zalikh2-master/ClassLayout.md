## Class Layout
This doc will be an overview of how I plan to structure my project.

### ofApp
The ofApp class will handle the bulk of all the drawing of shapes. It will also contain the initialization code for the GUI,
as well as event handlers that will dictate what happens when a GUI element is interacted with. ofApp will also make use of my 
external library, ofxBeat, by retrieving the audio data and magnitude to help with this drawing.

### Shape
Shape will be an abstract class to represent the different types of shapes I will have available in the audio visualizer. I plan to have
a circle, triangle, square, and star available for the user to pick. For each of those indiviudal shapes, they will inherit from the
Shape class. The Shape class will have an abstract method, draw(), which each shape will implement with its own instructions.

### Particle
The Particle class will be used to hold all the data related to a shape. While Shape only contains the information to draw(),
Particle will also have  information such as locations of the Shape, the color, rotation, and other attributes. Particles will
mainly interact with ofApp so that they are drawn correctly on the screen.
