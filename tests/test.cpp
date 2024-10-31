#include "../include/RenderEngine"

int main() {
    RenderEngine renderEngine(800, 500);

    renderEngine.setFramerate(60.0);
    Module module = Module(BoundingBox{100, 50, 100, 100}, renderEngine.getRenderer());
    renderEngine.addModule(std::make_shared<Module>(module));

    while(renderEngine.renderFrame()) {
    }

    return 0;
}
