# Running debug builds

This feature allows you to run XEX files built in debug mode or EXE files that can normally only run on Devkits.

It is recommended **NOT** to rename the plugin XEX file. Or if you do, make sure the name is 11-characters long **AT MOST!**, otherwise loading debug builds won't work. This is due to technical limitations.

## Tested games

### Call of Duty 4 - Pre Alpha 253

| File                   | Status             | Notes      |
| ---------------------- | ------------------ | ---------- |
| CoD3MP.exe             | :heavy_check_mark: |            |
| CoD3SP.exe             | :heavy_check_mark: |            |
| CoD3SP_fast_server.exe | :heavy_check_mark: |            |
| CoD3SP_profile.exe     | :x:                | Zone error |

### Call of Duty 4 - Pre Alpha 290

| File                   | Status             | Notes      |
| ---------------------- | ------------------ | ---------- |
| CoD3MP.exe             | :heavy_check_mark: |            |
| CoD3SP.exe             | :heavy_check_mark: |            |
| CoD3SP_fast_server.exe | :heavy_check_mark: |            |
| CoD3SP_profile.exe     | :x:                | Zone error |

### Call of Duty 4 - Pre Alpha 328

| File                   | Status             | Notes |
| ---------------------- | ------------------ | ----- |
| cod3mp.exe             | :heavy_check_mark: |       |
| cod3sp.exe             | :heavy_check_mark: |       |
| CoD3SP_fast_server.exe | :heavy_check_mark: |       |
| CoD3SP_profile.exe     | :heavy_check_mark: |       |

### Call of Duty Modern Warfare 2 - Pre Alpha 482

| File                    | Status             | Notes |
| ----------------------- | ------------------ | ----- |
| 1-iw4sp.exe             | :heavy_check_mark: |       |
| 2-iw4mp.exe             | :heavy_check_mark: |       |
| 3-iw4sp_fast_server.exe | :heavy_check_mark: |       |
| 4-iw4sp_demo.exe        | :heavy_check_mark: |       |
| 5-iw4mp_demo.exe        | :heavy_check_mark: |       |
| 6-iw4mp_fast_server.exe | :heavy_check_mark: |       |
| 7-iw4sp_demo_replay.exe | :heavy_check_mark: |       |
| 8-iw4mp_beta.exe        | :heavy_check_mark: |       |

### Call of Duty Modern Warfare 3 - Debug 419

| File                    | Status             | Notes |
| ----------------------- | ------------------ | ----- |
| 1-iw5sp.exe             | :heavy_check_mark: |       |
| 2-iw5mp.exe             | :heavy_check_mark: |       |
| 3-iw5sp_fast_server.exe | :heavy_check_mark: |       |
| 4-iw5sp_demo.exe        | :heavy_check_mark: |       |
| 5-iw5mp_demo.exe        | :heavy_check_mark: |       |
| 6-iw5mp_fast_server.exe | :heavy_check_mark: |       |
| 7-iw5sp_demo_replay.exe | :heavy_check_mark: |       |
| 9-iw5sp_screenshot.exe  | :heavy_check_mark: |       |
| a-iw5mp_screenshot.exe  | :heavy_check_mark: |       |

### Call of Duty Future Warfare (NX1)

| File                     | Status             | Notes       |
| ------------------------ | ------------------ | ----------- |
| 0-Convoy Test 1_5.mp.xex | :heavy_check_mark: |             |
| 0-Nightly MP maps.mp.xex | :x:                | Fatal crash |
| 0-Nightly SP maps.sp.xex | :heavy_check_mark: |             |
| 1-nx1sp.xex              | :x:                | Fatal crash |
| 2-nx1mp.xex              | :x:                | Fatal crash |
| 3-nx1sp_fast_server.xex  | :x:                | Fatal crash |
| 4-nx1sp_demo.xex         | :x:                | Fatal crash |
| 5-nx1mp_demo.xex         | :heavy_check_mark: |             |
| 6-nx1mp_fast_server.xex  | :x:                | Fatal crash |

> [!NOTE]
> You must **NOT** be signed into a profile to start the MW3 EXEs, otherwise it will display an error or crash because you have DLC content (at least Erosion, Aground and Terminal) installed for a different version of the game. Just start the EXE and then connect to a profile after.

### Call of Duty Ghosts - Alpha (May 9th build)

| File                    | Status             | Notes |
| ----------------------- | ------------------ | ----- |
| 1-iw6sp.exe             | :heavy_check_mark: |       |
| 2-iw6mp.exe             | :heavy_check_mark: |       |
| 3-iw6sp_fast_server.exe | :heavy_check_mark: |       |
| 4-iw6sp_demo.exe        | :heavy_check_mark: |       |
| 5-iw6mp_demo.exe        | :heavy_check_mark: |       |
| 6-iw6mp_fast_server.exe | :heavy_check_mark: |       |
