Overdrive
=========

Game engine, using C++11, Boost and openGL 4 technology

Current dependencies:
- openGL 4.5
- boost 1.62.0
- GLFW 3.1.2
- glew 1.13.0
- glm 0.9.7.2
- gli 0.7.0

[Window specific dependencies]
- xinput 1.4

The one dependency that isn't actually included in the repository is boost; 
it is assumed that it has been added to the global library paths. 

This engine is developed using Visual Studio in x64 mode, but care is taken to isolate platform specifc code.