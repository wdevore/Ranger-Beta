# Ranger-Beta
A c++17 continuation of Ranger-Alpha and FlutterRanger

This project was started in April 2025 and is a mixture of:

- [Ranger-Alpha](https://github.com/wdevore/Ranger-Alpha) was done in March 2016
- [Ranger-Go-IDE](https://github.com/wdevore/Ranger-Go-IGE) was done is 2020
- [SimpleOpenGL-Go](https://github.com/wdevore/SimpleOpenGL-Go) was done in 2020

# Checkmark ✔

# Tasks
* Complete shapes ✔
* Event system ✔
* Integrate Node system ✔
  * Zoom Node ✔
  * Dragging ✔
  * Space mapping ✔
* Zones
* Interpolation
* Fonts
  * Vector <-- **WORKING**
  * Bitmap
* Box2D
* Particles
* Filters
* Animation (tweening) *Using tanema's library*: https://github.com/tanema/gween

# Code documentation

## Shader
A Shader has a program associated with it. You construct a Shader using Vertex and Fragment shader programs.

# Atlas
An Atlas has a Shader associated with it. It contains vector shapes.

# Fonts

0x18 24 42 42 24 18 00 00
00011000 0x18
00100100 0x24
01000010 0x42
01000010 0x42
00100100 0x24
00011000 0x18
00000000 0x00
00000000 0x00