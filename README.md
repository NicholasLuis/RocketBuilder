# AERSP 424 Final Project


## Initial Setup:
1. Download all files
2. Using CMake, build the solution
3. Run the program


## This program has 2 distinct interactive menus:
1. Rocket Builder
2. TLE Loader


### ROCKET BUILDER
In the Rocket Builder menu, choose to either _Select from one of the Pre-Made Rockets_ or _Build your Own_

  1. Premade Rockets:
     Pick the file that corresponds to the rocket you wish to use.
       _Tip: You can make your own premade rocket by creating a .txt file and following the format given in ./ReferenceMaterial/Rocket_Data_Info.txt_
     
  2. Build your Own:
     a. Choose the number of stages you want your rocket to have (A tab will open up for every stage)
     b. Enter the values for the structural mass, fuel mass, and the engine's specific impulse for every stage
       _Tip: You can simulate the final payload by including a fuel weight of 0 in the final stage_
     c. Using the delta V information from the rocket (or the remaining stages), the computer will determine some potential orbital maneuvers


### TLE Loader
In TLE loader, choose either from _Load TLE from File_ or _Select from available TLE data_

  1. Loading TLE from file
     a. In the dialogue box, enter the full filepath and name of the TLE file to import into the program
     
  2. Select from available TLE data
     a. Choose from one of the example data files to see the data associated with that satellite
