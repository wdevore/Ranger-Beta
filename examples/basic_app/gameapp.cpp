#include <iostream>
#include <memory>

#include <gameapp.hpp>
#include <basic_scene_node.hpp>
#include <basic_shader.hpp>
#include <shape_generator.hpp>
#include <constants.hpp>

namespace Game
{
    GameApp::~GameApp()
    {
    }

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

    int GameApp::deconstruct()
    {
        std::cout << "GameApp::deconstruct" << std::endl;
        // atlas.dispose();

        return 1; // success
    }

    int GameApp::setup()
    {
        std::cout << "GameApp::setup" << std::endl;

        // Load any assets, for example vector font.

        nodeMan.initialize();

        return 1;
    }

    int GameApp::construct()
    {
        std::cout << "GameApp::construct game" << std::endl;

        auto env = environment;
        env->initialize(width, height);

        // Create a single scene (aka Node) to hold a square.
        basicScene = std::make_shared<BasicScene>("BasicScene");
        nodeMan.push(basicScene);

        // TODO Hack for now. Add a shape at this point.
        Core::ShapeGenerator generator{};
        // 0
        generator.generateUnitRectangle(Core::ShapeControls::Centered, Core::ShapeControls::Filled);
        Core::Shape &shape = generator.shape;
        env->atlas.addShape(shape.name, shape.vertices, shape.indices, shape.primitiveMode);

        // 1
        generator.generateUnitTriangle(Core::ShapeControls::Centered, Core::ShapeControls::Filled);
        shape = generator.shape;
        env->atlas.addShape(shape.name, shape.vertices, shape.indices, shape.primitiveMode);

        // 2
        generator.generateUnitHLine();
        shape = generator.shape;
        env->atlas.addShape(shape.name, shape.vertices, shape.indices, shape.primitiveMode);

        // 3
        generator.generateUnitVLine();
        shape = generator.shape;
        env->atlas.addShape(shape.name, shape.vertices, shape.indices, shape.primitiveMode);

        // 4
        generator.generateUnitPlus();
        shape = generator.shape;
        env->atlas.addShape(shape.name, shape.vertices, shape.indices, shape.primitiveMode);

        // 5
        generator.generateUnitCircle(12, Core::ShapeControls::Filled);
        shape = generator.shape;
        env->atlas.addShape(shape.name, shape.vertices, shape.indices, shape.primitiveMode);

        // 6
        generator.generateUnitArc(0.0, 45.0 * Core::DEGREES_TO_RADIANS, 6, Core::ShapeControls::Filled);
        shape = generator.shape;
        env->atlas.addShape(shape.name, shape.vertices, shape.indices, shape.primitiveMode);

        env->atlas.burn(true);
        env->atlas.use();

        env->atlas.setColor({1.0, 0.5, 0.0, 0.0});

        return 1;
    }

    void GameApp::render()
    {
        // Clear the colorbuffer
        glClearColor(bgClearColor.r, bgClearColor.g, bgClearColor.b, bgClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);

        // 1) Check projection and model

        // Render
        // nodeMan.visit(0.0, width, height);
        auto env = environment;

        Core::Matrix4 model{true};
        model.translate(250.0, 250.0, 0.0);
        model.scaleBy(100.0, 100.0, 1.0);
        env->atlas.setColor({1.0, 0.5, 0.0, 0.0});
        env->atlas.render(0, model); // rectangle

        model.toIdentity();
        // Order matters
        model.translate(150.0, 450.0, 0.0);
        model.scaleBy(100.0, 100.0, 1.0);
        model.rotate(45.0 * Core::DEGREES_TO_RADIANS);
        env->atlas.setColor({0.5, 1.0, 0.5, 0.0});
        env->atlas.render(1, model); // triangle

        model.toIdentity();
        model.translate(100.0, 100.0, 0.0);
        model.scaleBy(100.0, 100.0, 1.0);
        env->atlas.setColor({1.0, 0.5, 0.5, 0.0});
        env->atlas.render(2, model); // hline

        model.toIdentity();
        model.translate(300.0, 100.0, 0.0);
        model.scaleBy(100.0, 100.0, 1.0);
        env->atlas.setColor({1.0, 0.0, 0.5, 0.0});
        env->atlas.render(3, model); // vline

        model.toIdentity();
        model.translate(375.0, 275.0, 0.0);
        model.scaleBy(100.0, 100.0, 1.0);
        env->atlas.setColor({1.0, 1.0, 0.0, 0.0});
        env->atlas.render(4, model); // hplus

        model.toIdentity();
        model.translate(400.0, 150.0, 0.0);
        model.scaleBy(100.0, 100.0, 1.0);
        env->atlas.setColor({0.5, 0.5, 1.0, 0.0});
        env->atlas.render(5, model); // circle

        model.toIdentity();
        model.translate(500.0, 500.0, 0.0);
        model.scaleBy(100.0, 100.0, 1.0);
        env->atlas.setColor({0.5, 0.5, 1.0, 0.0});
        env->atlas.render(6, model); // arc

        // glBindVertexArray(0); // no need to unbind it every time
    }

    void GameApp::processIOEvent()
    {
    }

    int GameApp::update(double dt)
    {

        nodeMan.update(dt);

        return 1; // Successful update
    }

} // namespace Game
