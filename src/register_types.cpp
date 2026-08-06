#include "register_types.h"

#include "discord_p2p_mesh_node.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void InitializeDiscordP2PMeshModule(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	ClassDB::register_class<DiscordP2PMesh>();
}

void UninitializeDiscordP2PMeshModule(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}
}

extern "C" {
// Initialization entry point, referenced by the entry_symbol in discord_p2p_mesh.gdextension.
GDExtensionBool GDE_EXPORT DiscordP2PMeshLibraryInit(GDExtensionInterfaceGetProcAddress p_get_proc_address, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
	godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

	init_obj.register_initializer(InitializeDiscordP2PMeshModule);
	init_obj.register_terminator(UninitializeDiscordP2PMeshModule);
	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

	return init_obj.init();
}
}
