# DiscordP2PMesh-GD

Godot 4 GDExtension wrapper around [DiscordP2PMesh](https://github.com/HeatXD/DiscordP2PMesh),
a peer-to-peer mesh networking library signaled over a Discord lobby.

Registers a `DiscordP2PMesh` node class exposing signals/methods for Discord login, lobby chat,
and P2P messaging; internally it's a thin adapter over DiscordP2PMesh's C API (`dpmesh_*`) and
carries no Discord SDK or networking logic of its own.

## Building (Windows, Visual Studio)

1. `git submodule update --init --recursive` to pull in `godot-cpp` and `DiscordP2PMesh`.
2. Build `godot-cpp` (see its own README) and `DiscordP2PMesh` (see `DiscordP2PMesh/README.md`,
   including vendoring the Discord Social SDK there) for both Debug and Release.
3. Open `DiscordP2PMesh-GD.slnx` and build. The post-build step copies `DiscordP2PMesh`'s output
   (`discordp2pmesh.dll`, `discord_partner_sdk.dll`, `juice.dll`) next to the extension DLL.
4. Open `project.godot` in Godot 4.1+ to try the `test/` scene.
