# This is super, super, super experimental

- I do not garantee memory sae althgouh I am tring my best
- I do not promise compatibility w windows, though pretty sure apple is fine
- heavily borrows from my failed projects and existing projects online
- which means, this project is for fun ONLY.
https://github.com/andresrama/OpenGL-Cube
https://github.com/joyanrrix/Shoot-Game

## Important:

- Switch to glfw fix cyclical association issue from:
  https://github.com/123R3N321/myGames
and
  https://github.com/123R3N321/myFirstGame

## Problems, issues, future area of work:

- maybe switch back to sdl2
- definitely need more sprite and 
- need sound effect


## most important note:
- solved dependency on Cmake issue
- Zero library dependency (yes, NONE, because I used the dumb approach of throwing everythng into include)
  - however, there is one place where it can break:
    glad which loads all opengl funcs for us, is dependent on the version. I am using 4.6 here, so no problem
  - In case you got a newer version, go to https://glad.dav1d.de/ and generate a new glad loader
    - specs: c/c++, OpenGL, API only select gl, No dependency(Extension) needed.

## have fun!