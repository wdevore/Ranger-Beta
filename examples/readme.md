# Description
Note: most of the early examples are broken as the framework changed over time and I haven't fixed yet.

For the most part each project moves toward increasing complexity but not always. For example basic_vector_font is simpler that basic_font.
- basic_window (nothing but glfw)
- basic_triangle (extends by adding basic concepts of OpengGL)
- basic_app (adds and atlas)
- basic_node (integrates Nodes and Nodemanager)
- basic_font (adds basic pixel-like vector font)
- basic_vector_font (The second project to use UBOs. It was also were the 0x502 error was first discovered.)
- simple_shader (Debugging why i get a 0x502 gl error in more complex examples. 
This is straight copy of learning opengl's code) It doesn't show the 0x502 error.
The error was caused by not re-uploading uniforms when switching between shaders.
- simple_ubo (This has the first implementation of a UBO using the std140 layout rules.) It is the predecessor to basic_vector_font