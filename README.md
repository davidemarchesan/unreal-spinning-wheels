# Spinning Wheels - Unreal Engine Project

Multiplayer, TrackMania-inspired, Unreal Engine C++ Project.

[video]
> Rlease download: https://github.com/davidemarchesan/unreal-spinning-wheels/releases

## Commands

### Race

| | |
| --- | --- |
| Accelerate | Arrow Up |
| Brake | Arrow Down / Left Ctrl |
| Turn left | Arrow Left |
| Turn right | Arrow Right |
| Turbo | Space Bar |
| Canc | Restart lap |

### Editor

| | |
| --- | --- |
| Move camera | W / A / S / D |
| Rotate camera | Q / E |
| Select block to build | 1 - 9 |
| Build | Left mouse button |
| Rotate block | Mouse wheel |
| Cancel build | Right mouse button / Esc |

## Key features

- Multiplayer and Steam sessions support
- Car custom movement physics
- Deterministic lap times / Race mode
- Leaderboard / Checkpoints / Laptimes
- Track editor / Grid system / tree like menu navigation
- Server creation and tracks playlist
- Meshes / Blender

Missing features due to lack of time:
- vertical grid
- jumps and fly physics
- car active aerodynamics
- car skeleton and animations
- car skin customization
- track validation
- block validation

### Track editor

Like TrackMania, the player can create new tracks with a simple editor and test them on the fly. The player have 5 types of blocks at its disposal:
- standard
- turn
- start
- checkpoint
- finish

While previewing a block, the player can rotate it with mouse wheel.

The player can save the track (it'll be saved as simple json file) and edit it in the future.

### Grid system

When loading a track, in both race and editor mode, a grid system is in charge of reading a specific struct (which comes from reading the json file) that tells which block goes in which cell (X, Y) with a certain rotation (R). 

### Car custom movement physics

To create something like TrackMania's car movement, a custom movement component has been created which handle the power-slide, most important mechanic of the game which lets the player gain advantage on tight corners.

Curves: acceleration, braking and angular speed are determined by pre-defined custom curves (there are no gears, torque or traction systems).

Acceleration: determines how much the car accelerate at a certain speed.
Angular speed: determines how much the car turns at a certain speed.
Braking: determines how much brake force is being applied after time since starting braking (the more time braking, the more brake force)
Speed: is determined by summing forces like acceleration, braking or ground friction.
Velocity: direction vector. Determines where the car is moving.

Turbo: consumable that increases acceleration when active. Resets each lap.

The velocity vector actually tells where the car is moving. The car has three main modes:
- drive
- slide
- crash

#### Drive

On drive mode, the velocity vector is equal to the facing direction (or forward vector).

#### Slide

On slide mode, the velocity and facing direction are not the same. For example, on a right turn, while sliding, the car wants to turn right but the velocity vector is still pointing forward. However, the slide implies more friction forces applied to the velocity, while velocity direction is interpolated to the desired direction of the car.

When velocity vector and facing direction vector are the same, the car returns on drive mode.

#### Crash

The car enter crash mode when hitting a wall at high speed. The velocity is cut by a percentage and is changed to the hit point bounce vector. Also, the car starts spinning. When the spinning speed comes down to a controllable value, the player can retake control of the car on slide mode.

If the angle between velocity and impact normal is less than some pre-defined degrees, the car will slide along the wall, but still penalized on speed.

### Deterministic lap times

Lap time is determined by the number of simulated movements that occured to complete a lap. Car does not move each frame, but each custom pre-defined delta (120hz = 0.008333s or 8.333ms). If the difference between two frames is enough to contain two simulated movements, then two simulated movements will be computed.

This way, lap times are determined by the number of simulated movements multiplied by the pre-defined delta (1000 movements * 8.333ms = 8333ms = 8.333s).

#### Deterministic movement

Accordingly, the car reads a buffer of simulated movements and consume them, one by one. A single movement is constructed of:
- drive input (0/1)
- brake input (0/1)
- turn input (-1/0/+1)
- turbo input (0/1)

Right now the car moves because simulated movements are passed by the controller. But for replays or ghosts, we could just push a pre-defined buffer of simulated movements into the car pawn and it'll start moving without being directly controlled.

### Multiplayer and Steam

The race mode is multiplayer ready where everything is replicated and synced.

The race is mode is divided in main phases:
- waiting for players.
- racing: players can control their cars.
- podium: leaderboard is shown. Players can not control their cars.

The server holds the leaderboard and decide to accept lap times from players. Changes to the leaderboard are spread to players whom HUD will notify.

The player can choose to play solo (LAN), join a LAN race or start a Steam session. When creating a session (solo or Steam) the server will play all saved tracks for 5 minutes each.

### Meshes and Blender

Car and track blocks meshes are simple but custom-made by me. Just wanted to give it a try :)