# Stealth Game - A stealth game made in C

## Requirements

- [libmlv](http://www-igm.univ-mlv.fr/~boussica/mlv/)

Can be installed with `sudo apt-get install libmlv3-dev`

## Compilation

`make`

`make` will compile the default version of the game, the circles around the guardians are following the walls, this can use a lot of CPU. If you want to compile the version whit the default circles, use `make cirlce` after `make`, this will compile the game with the default circles.

You can use `make clean` to clean the project and `make mrproper` to clean the project and remove the executable.

## Execution

`./stealth`

## Controls

- `Z`, `Q`, `S`, `D` or `Arrow keys` to move
- `Space` to be invisible
- `Shift` to sprint
- `Escape` to quit at any time
