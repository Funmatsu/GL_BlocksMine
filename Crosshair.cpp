#include "Crosshair.h"

float crosshairSize = 10.0f;
float crosshairThickness = 2.0f;

void Crosshair::defineCrosshairGeometry() {
    crosshairVertices = {
        { centerX - crosshairSize, centerY }, { centerX + crosshairSize, centerY }, // horizontal line
        { centerX, centerY - 2 * crosshairSize }, { centerX, centerY + 2 * crosshairSize }  // vertical line
    };

    vers = {
        // Horizontal bar
        centerX - crosshairSize, centerY - crosshairThickness, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        centerX + crosshairSize, centerY - crosshairThickness, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        centerX + crosshairSize, centerY + crosshairThickness, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        centerX - crosshairSize, centerY + crosshairThickness, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

        // Vertical bar
        centerX - crosshairThickness/2, centerY - 2 * crosshairSize, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        centerX + crosshairThickness/2, centerY - 2 * crosshairSize, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        centerX + crosshairThickness/2, centerY + 2 * crosshairSize, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        centerX - crosshairThickness/2, centerY + 2 * crosshairSize, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f
    };
    
    inds = {
        // Horizontal bar
        0, 3, 2,
        2, 1, 0,
        // Vertical bar
        4, 7, 6,
        6, 5, 4
    };

    crosshair.createMesh(vers, inds, vers.size(), inds.size());
}

void Crosshair::drawCrosshair() {
    crosshair.renderMeshAsLines();
}