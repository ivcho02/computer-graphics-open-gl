# Rectangle Drawing Application

A simple MFC (Microsoft Foundation Classes) application for creating and managing rectangles by clicking two points.

## Description

This is a Windows application built with MFC that allows users to create rectangles interactively by clicking two points on the screen. The application supports creating multiple rectangles and deleting them by right-clicking.

## Features

- **Create Rectangles**: Click two points with the left mouse button to create a rectangle
  - First click sets the first corner point
  - Second click sets the opposite corner point and creates the rectangle
- **Delete Rectangles**: Right-click on a rectangle to delete it
- **Multiple Rectangles**: Support for up to 30 rectangles simultaneously
- **OpenGL Support**: OpenGL context is initialized (available for future enhancements)

## Usage

1. **Create a Rectangle**:
   - Click the left mouse button at the first corner position
   - Click the left mouse button again at the opposite corner position
   - A rectangle will be drawn between these two points

2. **Delete a Rectangle**:
   - Right-click anywhere inside a rectangle
   - The rectangle will be removed from the screen

## Requirements

- Microsoft Visual C++ (MFC)
- Windows operating system
- OpenGL libraries (for context initialization)

## Project Structure

- `GlmfcnView.cpp/h` - View class handling drawing and mouse events
  - `OnDraw()` - Renders all rectangles
  - `OnLButtonDown()` - Handles rectangle creation
  - `OnRButtonDown()` - Handles rectangle deletion
- `GlmfcnDoc.cpp/h` - Document class storing rectangle data
  - `rectangles[30]` - Array storing up to 30 rectangles
  - `rectanglesCount` - Current number of rectangles
  - `hasFirstPoint` - Flag indicating if first point is set
  - `firstPoint` - Coordinates of the first click point
- `Glmfcn.cpp/h` - Main application class

## Building

1. Open the project in Visual Studio
2. Build the solution (Build → Build Solution)
3. Run the application (Debug → Start Debugging or F5)

## Technical Details

### Rectangle Storage
- Rectangles are stored in a fixed-size array of `CRect` objects
- Maximum capacity: 30 rectangles
- Each rectangle is defined by two corner points

### Drawing
- Uses GDI (Graphics Device Interface) for rendering
- Rectangles are drawn with system button face color brush
- Double buffering is not implemented for GDI drawing

### OpenGL Context
- OpenGL context is initialized but not currently used for rendering
- Available for future 3D graphics enhancements

## License

This project is part of a computer graphics course work.

