#include <iostream>
#include <ostream>
#include "core/Application.h"
#include "scenes/MenuScene.h"
#include "scenes/MeshScene.h"
#include "scenes/PlayScene.h"
#include "scenes/TestScene.h"

int main() {
    try {
        const cbit::WindowConfig windowConfig{
            1200,
            800,
            "Fantasy Tactics",
            true,
            false,
            true
        };

        cbit::Application app(windowConfig);

        app.getSceneManager()->addScene("MenuScene", std::make_shared<MenuScene>());
        app.getSceneManager()->addScene("PlayScene", std::make_shared<PlayScene>());
        app.getSceneManager()->addScene("TestScene", std::make_shared<TestScene>());
        app.getSceneManager()->addScene("MeshScene", std::make_shared<MeshScene>());
        app.getSceneManager()->setActiveScene("PlayScene");

        app.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
