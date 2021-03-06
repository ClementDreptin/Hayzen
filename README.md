# Hayzen
Hayzen is a dynamic library (DLL) for RGH/Jtag/Devkit Xbox 360s that contains mods for different games.

## Features
Game detection:
![MW2 Multiplayer Detected](./resources/screenshots/mw2-xnotify.png)

A mod menu:
![MW2 Multiplayer Detected](./resources/screenshots/mw2-menu.png)

## Supported games
- Call of Duty: Modern Warfare 2 - TU9 Multiplayer (latest version)
- Call of Duty: Modern Warfare 2 - TU9 Special Ops (latest version)
- Call of Duty: Modern Warfare 2 - Alpha 482 Multiplayer
- Call of Duty: Modern Warfare 2 - Alpha 482 Special Ops
- Call of Duty: Modern Warfare 3 - TU24 Multiplayer (latest version)
- Call of Duty: Modern Warfare 3 - TU24 Special Ops (latest version)

## Installation

### Normal use
- Download the latest binary from the [releases](https://github.com/ClementDreptin/Hayzen/releases).
- Load the module either by setting it as a Dashlaunch plugin or by using an external tool to load modules, such as [ModuleLoader](https://github.com/ClementDreptin/ModuleLoader) :wink:.

### Development
Clone the repository and the submodule:
```
git clone --recursive https://github.com/ClementDreptin/Hayzen.git
```
Make sure your RGH/Jtag/Devkit is set up in Xbox 360 Neighborhood to be able to deploy the XEX file to your console and build successfully. Or just exclude the deployment to console from the build if you don't want to deploy (`Configuration Properties > Console Deployment > General > Excluded From Build`).
