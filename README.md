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

## Font tasks
- For each 0 pixel we render with a dark color (to be done later)
- For each 1 pixel we render with a light color.

StaticMonoAtlas:

We add a new shape that is built using 8x8=64 rectangles. Each rectangle is the shape is displaced by a gap. All rectangles fill the unit space.
We then create Indices for each character. In the first draft only pixels a "1" are drawn. For example, to render an exclamation point you would need 5 indice definitions (aka offsets in the EBO). Basically we keep adding indices at the next available offset.

We also keep a map of ```<String,int-offset>``` so we can lookup the characater index, the map is stored in darkrose_bitmap_font (dbf). dbf is given an ```evn``` for referencing the atlas. It then uses a generator to create rectangle unit squares. dbf then generates indices for each character.
once complete the shape can be added to the atlas and then can be initialized env.postInitiailize();
Note: that postInitiailize bakes the atlas and this can only be done once all the shapes are present.

- In the render call we use a localModel that is composited with the incoming
model. The localModel has appended a translation equal to 1.0.
- The fontNode has a scale equal to font size.

