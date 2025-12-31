/**
 * @file    MeshScene.h
 * @brief   Mesh scene for rendering various 3D meshes.
 * @author  Nur Akmal bin Jalil
 * @date    2025-12-31
 */

#ifndef FANTASYTACTICS_MESHSCENE_H
#define FANTASYTACTICS_MESHSCENE_H

#include "core/Scene.h"

class MeshScene final : public cbit::Scene {
public:
    void initialize() override;

    void update(float deltaTime) override;

    void render() override;
};


#endif //FANTASYTACTICS_MESHSCENE_H
