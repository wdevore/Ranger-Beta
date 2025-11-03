#include <iostream>
#include <memory>

#include <gameapp.hpp>
#include <basic_scene_node.hpp>
#include <shader.hpp>
#include <shape_generator.hpp>
#include <darkrose_bitmap_font.hpp>
#include <basic_vector_font.hpp>
#include <particle_font_atlas.hpp>

namespace Game
{
    GameApp::~GameApp()
    {
    }

    void GameApp::setProjection()
    {
        const float near{-1.0};
        const float far{1.0};
        const float w = static_cast<float>(Core::deviceWidth);
        const float h = static_cast<float>(Core::deviceHeight);

        Core::OrthoProjection projection{};

        projection.setProjection(h, 0.0, // bottom-left
                                 0.0, w, // top-right
                                 near, far);

        // Set global projection.
        Core::projection = projection.getMatrix();
    }

    /// @brief This is the earliest method called.
    /// mostly for testing and debugging.
    void GameApp::preSetup()
    {
    }

    // Called after App has initialized
    int GameApp::setup()
    {
        std::cout << "GameApp::setup" << std::endl;

        setProjection();

        // Load any assets, for example vector font.

        nodeMan.initialize();

        return 1;
    }

    // Called when it is time to contruct your game nodes
    int GameApp::construct()
    {
        std::cout << "GameApp::construct game" << std::endl;

        // ----------------------------------------------------------
        // Setup environment  ======= OpenGL is valid here ==========
        // ----------------------------------------------------------
        shapesAtlas.initialize();

        // ------------------------- Bitmap Vector Font Atlas -----------------
        // Create Darkrose pixel vector font
        // bitmapFontAtlas.initialize();
        // bitmapFontAtlas.finalize();
        // bitmapFontAtlas.scale.set(25.0, 25.0, 1.0);
        // bitmapFontAtlas.position.set(50.0, 50.0);
        // bitmapFontAtlas.horzOffset = 25.0;

        // ------------------------- Vector Vector Font Atlas -----------------
        vectorFontAtlas.initialize(Core::SimpleVectorFont::fontData4);
        vectorFontAtlas.finalize();

        // ------------------------- Particle Font Atlas -----------------
        // text = "FPS: $%&#@";
        // textToList(text);
        particleFontAtlas.initialize();
        particleFontAtlas.finalize();
        particleFontAtlas.scale.set(25.0, -25.0, 1.0);
        particleFontAtlas.position.set(25.0, Core::deviceHeight - 25.0);
        particleFontAtlas.horzOffset = 25.0;
        particleFontAtlas.particleSize = 3.0f;

        // Optional: Moving the camera (aka viewspace, aka viewMatrix)

        // ----------------------------------------------------------
        // Setup Scenes and Nodes
        // ----------------------------------------------------------
        // Create a single scene (aka Node) to comply with framework.
        // The scene simply sets the background color.
        basicScene = std::make_shared<BasicScene>("BasicScene");
        basicScene->build(nodeMan);

        nodeMan.push(basicScene);

        // ---------- Primitive debug tester
        int squareId = shapesAtlas.addSquare();
        int triId = shapesAtlas.addTriangle();
        shapesAtlas.finalize();
        _squareShape = shapesAtlas.getShapeById(squareId);
        _triangleShape = shapesAtlas.getShapeById(triId);

        // _triangle.configure();
        // _square.configure();

        // ----------------------------------------------------------
        // Finally set up the UBO
        // ----------------------------------------------------------
        // uboBuffer.configure({_triangle.getProgram(), _square.getProgram()});
        // uboBuffer.configure({_square.getProgram()});
        // uboBuffer.associateShaderIds({shapesAtlas.program(), _triangle.getProgram()});
        uboBuffer.associateShaderIds({shapesAtlas.program(),
                                      // bitmapFontAtlas.program(),
                                      particleFontAtlas.program(), // Particle-based bitmap
                                      vectorFontAtlas.program()});

        // setupSmoothing();

        return 1;
    }

    // Allows app to check if the app has been setup properly.
    int GameApp::verifyConfigured()
    {
        std::cout << "GameApp::verifyConfigured" << std::endl;

        if (nodeMan.nodes.size() < 1)
        {
            std::cout << "App is not configured correctly. There must be at least 1 node." << std::endl;
            return -1;
        }

        return 0;
    }

    // Called when the game loop exits and it is time to cleanup
    int GameApp::deconstruct()
    {
        std::cout << "GameApp::deconstruct" << std::endl;

        // bitmapFontAtlas.dispose();

        vectorFontAtlas.dispose();
        particleFontAtlas.dispose();

        uboBuffer.dispose();
        shapesAtlas.dispose();

        // ---------- Primitive debug tester

        // _triangle.dispose();
        // _square.dispose();

        return 1; // success
    }

    int GameApp::update(double dt)
    {
        nodeMan.update(dt);

        return 1; // Successful update
    }

    void GameApp::render()
    {
        // Render
        nodeMan.visit(0.0, width, height);

        // ------------ Model to use
        Core::Matrix4 model{true};

        // ---------- Primitive debug tester
        // model.toIdentity();
        // model.translate(325.0, 325.0, 0.0);
        // model.scaleBy(100.0, 100.0, 1.0);
        // _triangle.begin();
        // _triangle.use();
        // _triangle.setColor({1.0, 0.5, 0.0, 0.0});
        // _triangle.render(model);

        // _square.begin();
        // _square.use();
        // _square.setColor({1.0, 1.0, 1.0, 0.0});
        // _square.render(model);

        model.toIdentity();
        model.translate(125.0, 125.0, 0.0);
        model.scaleBy(25.0, 25.0, 1.0);
        shapesAtlas.use();
        shapesAtlas.setColor({1.0, 1.0, 1.0, 0.0});
        shapesAtlas.render(_squareShape, model);

        model.toIdentity();
        model.translate(225.0, 225.0, 0.0);
        model.scaleBy(100.0, 100.0, 1.0);
        // We don't need to call begin/use because the object is part of the
        // current active atlas.
        // shapesAtlas.begin();
        // shapesAtlas.use();
        shapesAtlas.setColor({0.5, 1.0, 1.0, 0.0});
        shapesAtlas.render(_triangleShape, model);

        // bitmapFontAtlas.use();
        // bitmapFontAtlas.setColor({1.0, 0.5, 1.0, 0.0});
        // bitmapFontAtlas.renderText(textList);

        model.toIdentity();
        model.translate(400.0, 400.0, 0.0);
        // Gemini generated the fonts mirrored on both x and y axises, so I
        // need to multiply by negatives to flip them.
        model.scaleBy(25.0, -25.0, 1.0);
        // vectorFontAtlas.begin();
        // vectorFontAtlas.use();
        // vectorFontAtlas.setColor({1.0, 1.0, 0.5, 0.0});
        // vectorFontAtlas.renderChar('$', model);

        model.toIdentity();
        model.translate(400.0, 400.0, 0.0);
        model.scaleBy(25.0, 25.0, 1.0);
        particleFontAtlas.use();
        particleFontAtlas.setColor({1.0, 0.5, 0.0, 1.0});
        textToList(currentFPSText);
        particleFontAtlas.renderText(textList);
        particleFontAtlas.unUse();
        // particleFontAtlas.renderChar('S', model);
    }

    void GameApp::processIOEvent()
    {
        nodeMan.processEvent(ioEvent);
    }

    void GameApp::textToList(const std::string &text)
    {
        textList.clear();
        for (char c : text)
            textList.push_back(c);
    }

} // namespace Game
