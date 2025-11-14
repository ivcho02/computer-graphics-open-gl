# OpenGL Sphere Renderer

A 3D sphere visualization application with ambient shadow lighting and interactive mouse rotation.

## Description

This is an MFC (Microsoft Foundation Classes) application that uses OpenGL to render a 3D sphere with realistic lighting. The sphere can be rotated interactively by dragging with the mouse.

## Features

- **3D Sphere**: Rendering of a sphere using the GLU library
- **Ambient Shadow Lighting**: Realistic lighting with ambient, diffuse, and specular components
- **Interactive Rotation**: Rotate the sphere by dragging with the mouse
  - Horizontal movement rotates around the Y axis
  - Vertical movement rotates around the X axis

## Technical Details

### Lighting

The application uses OpenGL lighting system with:
- **Ambient Light**: Minimal ambient light for shadow effect
- **Diffuse Light**: Yellow diffuse light (1.0, 1.0, 0.0)
- **Specular Light**: White specular light for shine
- **Global Ambient**: Greenish ambient for additional shadow effect (0.1, 0.2, 0.1)

### Material

The sphere uses a material with properties:
- Ambient: (0.5, 0.5, 0.7) - bluish color
- Diffuse: (0.5, 0.5, 0.7) - same color for diffuse lighting

### Projection

Uses orthographic projection for simpler coordinate management.

## Usage

1. **Start**: Compile and run the application
2. **Rotation**: 
   - Click and hold the left mouse button
   - Drag the mouse to rotate the sphere
   - Release the button to stop rotation

## Requirements

- Microsoft Visual C++ (MFC)
- OpenGL libraries
- GLU (OpenGL Utility Library)
- Windows operating system

## Project Structure

- `GlmfcnView.cpp/h` - Main rendering logic and mouse event handling
- `GlmfcnDoc.cpp/h` - Document class storing window data
- `Glmfcn.cpp/h` - Main application

## Building

1. Open the project in Visual Studio
2. Build the solution
3. Run the application

## License

This project is part of a computer graphics course work.
