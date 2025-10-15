# archECS
## Project Overview
In this project, I am developing archECS, a lightweight game engine framework with a modular Entity-Component-System (ECS) architecture.
The goal is to provide a modular foundation for experimenting with real-time rendering, component-based logic, and engine architecture design.

Currently, the project targets Windows and integrates a DirectX 11 renderer with a minimal abstraction layer (IRenderer, IWindow, IShader...) 
to facilitate future expansion toward cross-platform graphics APIs such as Vulkan or OpenGL.

The engine architecture is divided into multiple modules:

- EngineCore: Core ECS implementation, layer stack, and event handling.
- RenderCore: Rendering abstraction layer and DirectX 11 backend for GPU resource management, shaders, and draw calls.
- App: Demo application showing hundreds of moving cubes, how amazing!

The framework also integrates a custom math library and a custom memory management system, both developed as part of my Master’s program in Computer Game Development.

The long-term objective is to evolve archECS into a cross-platform, component-driven framework for graphics and simulation researches.

## First Results
