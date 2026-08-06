# DiscordP2PMesh-GD
### Godot 4 GDExtension for Discord powered peer to peer networking

A GDExtension wrapper around [DiscordP2PMesh](https://github.com/HeatXD/DiscordP2PMesh) that connects players directly to each other, without you running any servers.

## Project Goals
### Done
- `DiscordP2PMesh` node, usable as an autoload or a scene node
- Discord login with `application_id` as an editor property
- Configurable STUN/TURN servers from the Inspector
- Lobby signals
	- `lobby_joined`, `lobby_join_failed`, `lobby_left`
	- `lobby_message_received`, `lobby_member_joined`, `lobby_member_left`
- Peer to peer signals
	- `peer_connected`, `peer_disconnected`, `peer_data_received`
- Test scene showing lobby chat and P2P chat side by side
- Automated builds for Windows, Linux and macOS

### Maybe Later
- A `MultiplayerPeer` implementation so Godot's high level networking works on top of it

## Getting Started
### Docs
- Look at [`test/main.gd`](test/main.gd), it drives every signal and method the node exposes
- The underlying C API is documented in the core library's [`discord_p2p_mesh.h`](https://github.com/HeatXD/DiscordP2PMesh/blob/master/include/discord_p2p_mesh.h)

One thing worth knowing: sending to a peer for the first time only starts the connection, that first payload is not delivered. Wait for `peer_connected` before sending anything you care about.

## Building DiscordP2PMesh-GD
### Prerequisites
To build the extension, make sure you have the following installed:

1. **CMake** (version 3.16 or higher)
2. **C++ Compiler** with C++20 support:
   - **GCC** or **Clang** (Linux/macOS)
   - **MSVC** (Visual Studio) for Windows
3. **The Discord Social SDK**, see [DiscordP2PMesh/thirdparty/README.md](https://github.com/HeatXD/DiscordP2PMesh/blob/master/thirdparty/README.md). It's proprietary so it can't be shipped in either repo, you download it once and drop it into the submodule.
4. **Godot 4.1 or newer** to actually run it

Everything else, godot-cpp, the core library and libjuice, comes along as submodules, so there is no package manager to set up.

### Step-by-Step Instructions

#### 1. Clone the Repository
Both `godot-cpp` and `DiscordP2PMesh` come along as submodules:

```sh
git clone --recurse-submodules https://github.com/HeatXD/DiscordP2PMesh-GD.git
cd DiscordP2PMesh-GD
```

#### 2. Add the Discord Social SDK
Drop it into `DiscordP2PMesh/thirdparty/discord_social_sdk/` as described in the core library's thirdparty README.

#### 3. Configure Build Options
The build takes godot-cpp's own options, the useful one being which Godot target you're building for:

- `GODOTCPP_TARGET`: `template_debug` (default) or `template_release`.

```sh
cmake -S . -B build -DGODOTCPP_TARGET=template_debug
```

#### 4. Build the Project
```sh
cmake --build build
```

The extension is written straight into `bin/`, together with the Discord SDK runtime it needs, which is exactly where `discord_p2p_mesh.gdextension` expects to find them. Visual Studio can build this directly through File, Open, Folder.

#### 5. Open the Project
Open `project.godot` in Godot and run it, the test scene is the main scene.

### Build Output
- **Extension**: `bin/libdiscordp2pmesh.<platform>.<target>.<arch>.<ext>`
- **Runtime dependency**: `discord_partner_sdk`, copied next to it. The core library and libjuice are linked in statically.

---

## License
DiscordP2PMesh-GD is licensed under the BSD-2-Clause license
[Read about it here](https://opensource.org/license/bsd-2-clause).

The Discord Social SDK is not covered by that license and is not redistributed here, it remains subject to Discord's own developer terms.
