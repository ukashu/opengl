## Usage
1. Install packages
```
> sudo apt update
> sudo apt install build-essential
> sudo apt install libgl-dev libglfw3-dev
```
2. Download zip from https://gen.glad.sh/ I chose gl version 3.3, common and compatability and also marked 'loader' at the bottom. Extract
3. Copy files to match this folder structure:
```
zad2/
├── main.c
├── linmath.h
├── gl.c
└── glad/
    └── glad.h
    └── khrplatform.h
```
4. Compile
```
gcc main.c gl.c -o prism -I. -Iglad/ -lglfw -lGL -lm
```