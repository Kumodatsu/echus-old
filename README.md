# Echus
An exploration of real-time sound synthesis.

Note that this was just a personal project made for fun.
It is certainly not production worthy, but perhaps it can serve as inspiration
or provide some educational value.
I have stopped working on this project in favor of a different sound synthesis
project.

## Building and running
All commands given must be run from the repository's root folder, i.e. the
folder that contains this README file, unless otherwise specified.
Depending on your platform and shell you may also have to substitute any `/` in
paths with `\`.

### Prerequisites
- [CMake](https://cmake.org/)
- A C++ build environment, like g++ and GNU Make or Visual Studio.

### Building
Run these commands once the first time you're building the project:

    git submodule update --init --recursive
    mkdir build

Then, run the following two commands whenever you build the project:

    cmake -G [generator] -S . -B build
    cmake --build build

Here, `[generator]` corresponds to the build environment you wish to use, for
example `"MinGW Makefiles"` or `"Visual Studio 16 2019"`.
Run `cmake --help` to see which generators are available to you.
See the [CMake documentation](
    https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html
) for more information if you're unsure.

### Running
Run the following command after building:

    ./build/echus-exe
