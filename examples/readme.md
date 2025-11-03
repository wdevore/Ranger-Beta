# Description
Note: most of the early examples are broken as the framework changed over time and I haven't fixed yet.

# Projects
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

# Standard app startup sequence

**main()**:
1) Construct GameApp
2) Initialize GLFW via *App::initialize*. This is standard for all apps as all apps need GLFW.
    1) Initialize GLFW
    2) Call abstract *setup* to allow child GameApps to perform a setup prior to window creation. For example, initializing the **NodeManager**.
3) Create GLFW window via *App::createWindow*.
4) Call *App::configure()*. 
    1) Bind Inputs to callbacks, for example, Keys, Mouse etc...
    2) Load OpenGL functions.
        - <span style='color: yellow; background-color: gray;'>======= OpenGL functions can now be called ========</span>
    3) <span style='color: orange;'>**First OpenGL call**</span>: *glViewport(...)*
5) Call *GameApp::construct()*, it overrides App::construct
    1) Call *Environment::initialize()* passing window width and height. Environment contains objects that most games require, for example, perhaps a common font Atlas. If you app needs specific objects then you should place them in the GameEnvironment child object, for example, custom fonts or game wide assests or functionality.
    2) Construct any **Node**s and push them onto the Node stack.
    3) Call *Environment::postInitialize()*
6) Call *GameApp::run()*. Game doesn't end until the method returns.

# Shaders
An Atlas is a collection of vertices and indices. Each Atlas inherits from **BaseAtlas**.

Each Atlas has a set of common attributes that all Atlases have:
- Model

Each inherited Atlas has specific attributes:
- Color (via "fragColor")

Both the projection and view matrices are shared amoung the shaders via a UBO. They only need to be set once because our game isn't changing either of them. Using the UBO these matrices are shared amoung the shaders via a single bind point.

