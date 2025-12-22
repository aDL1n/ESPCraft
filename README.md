<h1 align="center">ESPCraft</h1>
A Minecraft clone designed specifically to run on the ESP32 in conjunction with an ILI9341-based display module.

> [!WARNING]
> The project is in a very early stage of development and is far from becoming a fully playable copy of the game.

### Features
Graphics engine for rendering
Simple flat world generator
Finding the block the camera is looking at
Controlling the camera with a joystick

### Limitations
No ability to directly interact with the world
No entities
Maximum number of chunks: 7 (due to the fact that there is no saving to memory and the world is stored entirely in RAM)

### TODO 
Add button support to scl
Add block placement and inventory
Add entity (add player first)
Add world data storage to LittleFS
Improve performance and rewrite the graphics engine
