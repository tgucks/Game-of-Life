# Conway's Game of Life Implementation in C++

This program is a simple implementation of the cellular automaton known as the "Game of Life" first created by John Conway in 1970. This implementation of the game utilizes command-line output to display the game board which updates and refreshes once every ~10 milliseconds. Rules and more information on the game can be found here: https://conwaylife.com/wiki/Conway%27s_Game_of_Life.

## Installation

In order to clone the repository and run this program on your local machine, please follow the instructions located at: https://docs.github.com/en/github/creating-cloning-and-archiving-repositories/cloning-a-repository.

Once cloned, please compile the single .cpp file and run the resulting executable.

## Usage

The program will first ask you if you would like for it to randomly generate a 50x50 game board or if you would like to provide a sample input file containing a "Game of Life" pattern. Press the letter 'y' for a random generation or 'n' to provide an input file.

The repository contains a few sample input files. If you would like to use one of these files, enter a file name like "sample_input/GosperGliderGun.txt". More patterns can be found here: https://www.conwaylife.com/ref/lexicon/lex.htm.

## Customization

The game board is currently set to have dimensions of 50x50. This can be adjusted by editing the global variables "globalNumRows" and "globalNumColumns".

The game can be sped up or slowed down by changing the argument of the Sleep() function within main(). Increase the argument value for a slower refresh rate and decrease it for a faster refresh rate.
