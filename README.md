# Game Hacking PoC 

This project was built as a learning exercise to implement a basic memory scanning/edting tool, common in Game Hacking, such as CheatEngine.


## How it works
The project is made of two different components: 
- Game
- CheatCLI

(I know I suck at naming, it's a actually a sign of programming seniority.)

The Game component is a simple implmentation revolving arround creating a Player structure with multiple fields.
This is the struct that the CheatCLI will attempt to find and modify*.


The CheatCLI mimicks a CheatEngine type of tool where it allows to scan a Process specified by the user and locate values within it.


## Basic usage:
The usage is pretty self-explanatory, launch both programs and attempt to locate a value in memory.
![Alt text](/screenshot.png?raw=true "Flow")


## TODO

- Add memory editing capability.
- UI?
- Maybe also better code documentation

