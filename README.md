# Hayzen

Hayzen is a dynamic library (DLL) for RGH/Jtag/Devkit Xbox 360s that contains mods for different games.

## Features

### Game detection

![MW2 Multiplayer Detected](./resources/screenshots/mw2-xnotify.png)

### A mod menu

![MW2 Multiplayer Detected](./resources/screenshots/mw2-menu.png)

#### Supported games

-   Call of Duty: Modern Warfare 2 - TU9 Multiplayer (latest version)
-   Call of Duty: Modern Warfare 2 - TU9 Special Ops (latest version)
-   Call of Duty: Modern Warfare 2 - Pre Alpha 482 Multiplayer
-   Call of Duty: Modern Warfare 2 - Pre Alpha 482 Special Ops
-   Call of Duty: Modern Warfare 3 - TU24 Multiplayer (latest version)
-   Call of Duty: Modern Warfare 3 - TU24 Special Ops (latest version)

### Running debug builds

This feature can be enabled from the Settings tab of the mod menu and is persistent accross reboots. It allows you to run XEX files built in debug mode or EXE file that can normally only run on Devkits, so no need for RGLoader anymore. The following EXEs were tested

#### Call of Duty 4 - Pre Alpha 253

| File                   | Status        | Notes      |
| ---------------------- | ------------- | ---------- |
| CoD3MP.exe             | :heavy_check: |            |
| CoD3SP.exe             | :heavy_check: |            |
| CoD3SP_fast_server.exe | :heavy_check: |            |
| CoD3SP_profile.exe     | :x:           | Zone error |

#### Call of Duty 4 - Pre Alpha 290

| File                   | Status        | Notes      |
| ---------------------- | ------------- | ---------- |
| CoD3MP.exe             | :heavy_check: |            |
| CoD3SP.exe             | :heavy_check: |            |
| CoD3SP_fast_server.exe | :heavy_check: |            |
| CoD3SP_profile.exe     | :x:           | Zone error |

#### Call of Duty 4 - Pre Alpha 328

| File                   | Status        | Notes |
| ---------------------- | ------------- | ----- |
| cod3mp.exe             | :heavy_check: |       |
| cod3sp.exe             | :heavy_check: |       |
| CoD3SP_fast_server.exe | :heavy_check: |       |
| CoD3SP_profile.exe     | :heavy_check: |       |

#### Call of Duty Modern Warfare 2 - Pre Alpha 482

| File                    | Status        | Notes |
| ----------------------- | ------------- | ----- |
| 1-iw4sp.exe             | :heavy_check: |       |
| 2-iw4mp.exe             | :heavy_check: |       |
| 3-iw4sp_fast_server.exe | :heavy_check: |       |
| 4-iw4sp_demo.exe        | :heavy_check: |       |
| 5-iw4mp_demo.exe        | :heavy_check: |       |
| 6-iw4mp_fast_server.exe | :heavy_check: |       |
| 7-iw4sp_demo_replay.exe | :heavy_check: |       |
| 8-iw4mp_beta.exe        | :heavy_check: |       |

#### Call of Duty Modern Warfare 3 - Debug 419

| File                    | Status        | Notes |
| ----------------------- | ------------- | ----- |
| 1-iw5sp.exe             | :heavy_check: |       |
| 2-iw5mp.exe             | :heavy_check: |       |
| 3-iw5sp_fast_server.exe | :heavy_check: |       |
| 4-iw5sp_demo.exe        | :heavy_check: |       |
| 5-iw5mp_demo.exe        | :heavy_check: |       |
| 6-iw5mp_fast_server.exe | :heavy_check: |       |
| 7-iw5sp_demo_replay.exe | :heavy_check: |       |
| 9-iw5sp_screenshot.exe  | :heavy_check: |       |
| a-iw5mp_screenshot.exe  | :heavy_check: |       |

**Note**: You must **NOT** be signed into a profile to start the MW3 EXEs, otherwise it will display an error or crash because you have DLC content (at least Erosion, Aground and Terminal) installed for a different version of the game. Just start the EXE and then connect to a profile after.

## Installation

### Normal use

-   Download the latest binary from the [releases](https://github.com/ClementDreptin/Hayzen/releases).
-   Load the module either by setting it as a Dashlaunch plugin or by using an external tool to load modules, such as [ModuleLoader](https://github.com/ClementDreptin/ModuleLoader) :wink:.

### Development

Clone the repository and the submodule:

```
git clone --recursive https://github.com/ClementDreptin/Hayzen.git
```

Make sure your RGH/Jtag/Devkit is set up in Xbox 360 Neighborhood to be able to deploy the XEX file to your console and build successfully. Or just exclude the deployment to console from the build if you don't want to deploy (`Configuration Properties > Console Deployment > General > Excluded From Build`).
