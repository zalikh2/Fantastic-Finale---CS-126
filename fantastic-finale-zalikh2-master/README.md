## openFrameworks Audio Visualizer
This program is an audio visualizer for my final project in CS126 at UIUC. An audio visualizer is a tool that create different graphics
and visualizations based on the data from the audio file being played and/or some user input and settings. This was created using
the standard openFrameworks library functions, as well as the external plugins ofxDatGui and ofxBeat.

### Features
This audio visualizer is very customizable. It includes the following features for each audio particle: 
 - Toggle particle drawing on/off
 - Rotation in the X, Y, and Z axes
 - Adjust center location
 - Pick 2 colors for the particle to swtich between
 - Adjust time taken to switch between colors
 - Create particles centered around location
 - Number of particles to draw
 - Choose from selection of shapes
 - Adjust the maximum size
 
![Audio Visualizer Screenshot](https://i.ibb.co/R6NMJQL/Audio-Visualizer-SC.jpg)

 For the main settings, the background can also interpolate between two user-chosen colors based on the audio file's volume,
 and the smoothing factor of the drawn shapes can also be chosen. There are also 3 songs to choose from.

 ### Instructions to run
 In order to run this project with the current files, you will need Visual Studio 2017 or 2019. You will also need to download
 the bin folder found [here](https://drive.google.com/drive/folders/1hla8E7bUiESwOggoaVmpZhP8lwv9DGBA?usp=sharing). This folder
 contains the data needed for ofxDatGui as well as the song files. Once everything has been downloaded simply click start and the 
 program should run.