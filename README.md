# R-TYPE
## What is R-TYPE?
Welcome to R-Type! the project goal is to recreate a mini-arcade shooters game from 1987 named R-Type, and adding a multiplayer mode to it.
For the graphical interface, we used SMFL.
For the client and the server, we used ASIO.


## Build the Project

First you need to install [CMake](https://cmake.org/download/) and [Conan](https://conan.io/downloads.html).

For this project you need to create a profile with conan that uses C++11 as the minimum C++ standard.

You need to add a remote for Conan as a dependency used in this project requires it, so just run:
```shell
conan remote add bincrafters https://bincrafters.jfrog.io/artifactory/api/conan/public-conan
```

Then you need to install the dependencies for the project using Conan.
We recommend creating a build folder at the root of the repository in which everything will.

To build, go in the build folder and execute the following commands :
```shell
conan install ..
cmake ..
cmake --build .
```

## Run the Project

You first need to start the server on a device.
Then you can open multiple clients. When you are on the login screen, you can login by providing the `ip` adress of the server, and a `port`.

The server is multithreaded and can handle 8 games at a time.

The clients can connect from port 6000 to 6007

If a server is unjoinable, a game may be already started on the port.

After login, you will be invited to enter a username.

After that you will be redirected on the lobby page, where you can wait others players to join, or start a solo game.

## Requirements
### Mandatories

✅ One-to-four player game, using a client/server architecture

✅ The client must display the game and handle player input.

✅ You MUST use the SFML for client-side

✅ The project MUST be OS independent. It has to compile and run in a similar manner on Unix system and Windows systems.

✅ It MUST be built using a [CMake] and dependencies MUST be handled using [conan].

✅ The client MUST display a slow horizontal scrolling background representing space with stars, planets

✅ The starfield scrolling must NOT be tied to the CPU speed. Instead, you MUST use timers.

✅ Players MUST be able to move using the arrow keys.

✅ The server MUST be multi-threaded.

✅ If a client crashes for any reason, the server MUST continue to work

✅ The four players in a game MUST be blue, red, yellow and green, respectively.

✅ You MUST design a binary protocol for client/server communications.

✅ You MUST use UDP for communications between the server and the clients

✅ You MUST document your protocol. The documentation MUST be an RFC

✅ Your RFC MUST be formatted like an official RFC.

✅ You MUST write the RFC in ASCII format. Postscript format is optional.

✅ You MUST respect standard RFC keywords as described in RFC 2119.

✅ There MUST be Bydos slaves in your game.

✅ Each kind of monster MUST be a dynamic library that can be loaded by your server without having to restart it.

✅ You MUST write your own API to interact with those libraries.

✅ Monsters MUST be able to spawn randomly on the right of the screen.

✅ The server MUST notify each client when a monster spawns, is destroyed, fires, kills a player, and so on...

✅ You MUST use the SFML (2.5.1) on the client side.

✅ Any solution to a problem MUST be object-oriented.

### May

❌ You MAY use Boost (v1.77.0) for the client side ONLY.

✅ You MAY use Asio (v1.20.0) for your server.

## Libraries
You must have the `SFML` for the UI side.
For the client and the server, we are using `ASIO`.

## Documentation

[![doc](https://img.shields.io/badge/Code_Documentation-pdf-red.svg)](doc/code_documentation.pdf)  
[![doc](https://img.shields.io/badge/Network_Protocol-txt-white.svg)](doc/network_protocol.txt)

## Authors
  - [Martin Olivier](https://github.com/tocola)
  - [Sean Morton](https://github.com/Seanmrt)
  - [Nicolas Allain](https://github.com/Nirasak)
  - [Maxime Sarres](https://github.com/XriM)
