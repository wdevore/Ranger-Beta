# Description pixel vector font
The font is constructed of a fixed set of vertices. The total is 8*8 pixels where each pixel is represented by 4 vertices = 256 vertices (VBO). Each pixel is displaced both vertically and horizontally (i.e. a gap). Each pixels is a Quad (2 triangles).

Each alpha character is defined by 8 bytes. Each byte is a row of bits and each bit represents one of the pixels either on or off. Each bit represents one a Quad. If the pixel is "1" the corresponding Quad is displayed.

Each character is 8*8 = 64 Quads. So for each char a list of indices are collected that represent which quads are displayed. All characters are defined statically.

## Atlases
An Atlas works by collecting *Shapes* where each shape contains a collection of vertices and indices.

Once all shapes are collected you need to transfer the vertices and indices into an OpenGL configuration. This is called *burning* and is done by a two step process: *shake* and *bake*.

## Shake
Shaking a shape is the process of copying the shape vertices and indices into to an OpenGL compatible **backing** arraya which are later copied into an OpenGL buffers.

## Bake
An Atlas is a collection of "shapes" and when all shapes are added the *bake* process should start. This is where the **backing** arrays are transferred to OpenGL buffers via the bind methods.

Finally the backing arrays are cleared as they are not needed once the engine is running.

|------------------|   |------------------|
|                  |   |                  |
|                  |   |                  |
|                  |   |                  |
|     geometric    |   |     geometric    |
|      shapes      |   |      shapes      |
|     vertices     |   |     indices      |
|------------------|   |------------------|
|                  |   |                  |
|                  |   |                  |
|      font        |   |      font        |
|     vertices     |   |     indices      |
|                  |   |                  |
|------------------|   |------------------|

## Pixel Vector Font
PV Fonts function much like geometric shapes except that there is a single group of vertices that represent an 8x8 grid of Quads, but there is an indices *group* for each character.

So for each character we iterate through each byte's bits and determine which quads are displayed. This means we calculate 4 indices for each pixel and group them together, and we do this 64 times for each character. Then for the next character we generate a new group of indices. We continue until all characters have a group of indices. Note all characters are done upfront, we don't build indices as we need them, they are all cache in an EBO buffer.

An example for the number 1 if represented by a vertical line would look like this:
00000000
00011000
00011000
00011000
00011000
00011000
00011000
00000000

Above we would have 12 indices groups.

Then for the next char, for example 0, we would pack right next to it 12+16=28 indices.
00000000
01111110
01100110
01100110
01100110
01100110
01111110
00000000

So the first char "1" is at offset *off1* and "0" is at offset *off2*. If the offset starts a 0 then *off2* is at 11, and the next char would be at offset 28+11=49 thus 48.


Note: All vertices and indices are *packed* into the same atlas that may also have geometric shapes.

An example, of an EBO grouping
```
vvvv_vvvv_vvvv_vvvv_vvvv_vvvv_vvvv_vvvv_vvvv_vvvv_vvvv_vvvv_vvvv_vvvv_vvvv_vvvv_vvvv_vvvv_vvvv_vvvv
|--------------------- A ------------------| |-------------------------- B -----------------------|
0 offset                                     8 offset
```

## Text
To render text we make draw element calls using the group offsets. For example, to render "Hello", this would require 5 draw calls. Each call would translate each character based on an accumulating offset, otherwise each char would appear on top of the other.




