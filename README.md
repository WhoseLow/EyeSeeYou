# EyeSeeYou
Project for a friend which displays an eyeball tracking faces within the frame of a camera. It's very hacky and janky but I was working on it and the logistics of setting it up in a real space up until the deadline so I didn't have time to finish. It would be cool if I went and completed/cleaned it up later but I most likely won't.

## Building youself
#### Dependancies
- a C++ compiler
- cmake
- SDL2
- opencv4

Arch Linux
```
yay -Sy gcc cmake opencv sdl2 git
```

### Install/build
```
git clone https://github.com/WhoseLow/EyeSeeYou.git
cd EyeSeeYou
cmake .
```
### Run
(from source dir)
```
./EyeSeeYou
```
