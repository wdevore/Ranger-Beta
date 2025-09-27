#include <iostream>
#include <memory>

#include <gameapp.hpp>
#include <basic_scene_node.hpp>
#include <basic_shader.hpp>
#include <shape_generator.hpp>
#include <constants.hpp>
#include <environment.hpp>
#include <darkrose_bitmap_font.hpp>

namespace Game
{
    GameApp::~GameApp()
    {
    }

    /// @brief This is the earliest method called.
    /// mostly for testing and debugging.
    void GameApp::preSetup()
    {
    }

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

    int GameApp::setup()
    {
        std::cout << "GameApp::setup" << std::endl;

        // atlas = std::make_shared<Core::StaticMonoAtlas>();

        // Load any assets, for example vector font.

        nodeMan.initialize();

        return 1;
    }

    int GameApp::construct()
    {
        std::cout << "GameApp::construct game" << std::endl;

        // ----------------------------------------------------------
        // Setup environment
        // Constructs Atlases etc.
        // ----------------------------------------------------------
        auto env = environment;
        env->initialize(width, height);

        // ----------------------------------------------------------
        // Setup Scenes and Nodes
        // ----------------------------------------------------------
        // Create a single scene (aka Node) to hold a square.
        // The Scene will have a background square for color.
        basicScene = std::make_shared<BasicScene>("BasicScene", env);
        basicScene->build(nodeMan);

        nodeMan.push(basicScene);

        // ----------------------------------------------------------
        // Setup Bitmap vector font
        // ----------------------------------------------------------
        // The font generates the vertices and indices
        auto font = std::make_unique<DarkroseBitmapFont>();
        font->build();

        // // Use vertices/indices to configure the atlas buffers
        env->fontAtlas.configureFrom(std::move(font));
        // ----------------------------------------------------------

        // ----------------------------------------------------------
        // OR: Test a single font char
        // ----------------------------------------------------------
        // Core::ShapeGenerator generator{};
        // // uint64_t character = 0x0808080800080000; // "!"
        // uint64_t character = 0x081E281C0A3C0800; // "$"
        // // uint64_t character = 0x3C42423E023C0000; // "9"
        // // uint64_t character = 0x00003A46423E0202; // "q"
        // // uint64_t character = 0x0000422214081060; // "y"
        // // uint64_t character = 0x1C103030101C0000; // "{"
        // // uint64_t character = 0x1818243C42420000; // "A"

        // generator.generateFontChar(character, Core::ShapeControls::Filled, GAPSIZE);
        // Core::Shape &shape = generator.shape;

        // env->atlas.addShape(shape);
        // ----------------------------------------------------------

        env->postInitialize();

        env->atlas.setColor({1.0, 0.5, 0.0, 0.0});

        return 1;
    }

    void GameApp::render()
    {
        // Render
        nodeMan.visit(0.0, width, height);

        auto env = environment;

        float xPos{50.0};
        const float horzOffset{45.0};
        int offset{0};

        env->fontAtlas.use();

        env->fontAtlas.setColor({1.0, 1.0, 0.0, 0.0});

        Core::Matrix4 model{true};
        model.translate(xPos, 50.0, 0.0);
        offset += horzOffset;
        model.scaleBy(50.0, 50.0, 1.0);
        env->fontAtlas.renderChar('H', {}, model);

        model.toIdentity();
        model.translate(xPos + offset, 50.0, 0.0);
        offset += horzOffset;
        model.scaleBy(50.0, 50.0, 1.0);
        env->fontAtlas.renderChar('e', {}, model);

        model.toIdentity();
        model.translate(xPos + offset, 50.0, 0.0);
        offset += horzOffset;
        model.scaleBy(50.0, 50.0, 1.0);
        env->fontAtlas.renderChar('l', {}, model);

        model.toIdentity();
        model.translate(xPos + offset, 50.0, 0.0);
        offset += horzOffset;
        model.scaleBy(50.0, 50.0, 1.0);
        env->fontAtlas.renderChar('l', {}, model);

        model.toIdentity();
        model.translate(xPos + offset, 50.0, 0.0);
        offset += horzOffset;
        model.scaleBy(50.0, 50.0, 1.0);
        env->fontAtlas.renderChar('o', {}, model);

        env->fontAtlas.unUse();

        // env->atlas.use();
        // Core::Matrix4 modelR{true};
        // modelR.translate(50.0, 50.0, 0.0);
        // // If the font is upside down we can scale the Y axis by a negative.
        // // Instead I fixed the generator to iterate each from bottom to top.
        // modelR.scaleBy(25.0, 25.0, 1.0);
        // env->atlas.setColor({1.0, 1.0, 0.0, 0.0});
        // env->atlas.render(1, modelR); // Char font
        // env->atlas.unUse();
    }

    int GameApp::update(double dt)
    {

        nodeMan.update(dt);

        return 1; // Successful update
    }

    void GameApp::processIOEvent()
    {
        nodeMan.processEvent(ioEvent);
    }

    int GameApp::deconstruct()
    {
        std::cout << "GameApp::deconstruct" << std::endl;
        environment->dispose();

        return 1; // success
    }

} // namespace Game
