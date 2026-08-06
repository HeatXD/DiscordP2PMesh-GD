#include "discord_p2p_mesh_node.h"

#include <godot_cpp/variant/utility_functions.hpp>

#include <cstring>

using namespace godot;

DiscordP2PMesh::DiscordP2PMesh() {
}

DiscordP2PMesh::~DiscordP2PMesh() {
	if (_session) {
		dpmesh_destroy(_session);
	}
}

void DiscordP2PMesh::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_application_id", "application_id"), &DiscordP2PMesh::SetApplicationId);
	ClassDB::bind_method(D_METHOD("get_application_id"), &DiscordP2PMesh::GetApplicationId);
	ClassDB::bind_method(D_METHOD("set_stun_server_host", "host"), &DiscordP2PMesh::SetStunServerHost);
	ClassDB::bind_method(D_METHOD("get_stun_server_host"), &DiscordP2PMesh::GetStunServerHost);
	ClassDB::bind_method(D_METHOD("set_stun_server_port", "port"), &DiscordP2PMesh::SetStunServerPort);
	ClassDB::bind_method(D_METHOD("get_stun_server_port"), &DiscordP2PMesh::GetStunServerPort);
	ClassDB::bind_method(D_METHOD("set_turn_server_host", "host"), &DiscordP2PMesh::SetTurnServerHost);
	ClassDB::bind_method(D_METHOD("get_turn_server_host"), &DiscordP2PMesh::GetTurnServerHost);
	ClassDB::bind_method(D_METHOD("set_turn_server_port", "port"), &DiscordP2PMesh::SetTurnServerPort);
	ClassDB::bind_method(D_METHOD("get_turn_server_port"), &DiscordP2PMesh::GetTurnServerPort);
	ClassDB::bind_method(D_METHOD("set_turn_username", "username"), &DiscordP2PMesh::SetTurnUsername);
	ClassDB::bind_method(D_METHOD("get_turn_username"), &DiscordP2PMesh::GetTurnUsername);
	ClassDB::bind_method(D_METHOD("set_turn_password", "password"), &DiscordP2PMesh::SetTurnPassword);
	ClassDB::bind_method(D_METHOD("get_turn_password"), &DiscordP2PMesh::GetTurnPassword);

	ClassDB::bind_method(D_METHOD("login"), &DiscordP2PMesh::Login);
	ClassDB::bind_method(D_METHOD("is_ready"), &DiscordP2PMesh::IsReady);
	ClassDB::bind_method(D_METHOD("create_or_join_lobby", "secret"), &DiscordP2PMesh::CreateOrJoinLobby);
	ClassDB::bind_method(D_METHOD("leave_lobby"), &DiscordP2PMesh::LeaveLobby);
	ClassDB::bind_method(D_METHOD("send_lobby_message", "text"), &DiscordP2PMesh::SendLobbyMessage);
	ClassDB::bind_method(D_METHOD("send_to_peer", "peer_id", "data"), &DiscordP2PMesh::SendToPeer);
	ClassDB::bind_method(D_METHOD("get_current_user_id"), &DiscordP2PMesh::GetCurrentUserId);
	ClassDB::bind_method(D_METHOD("get_user_display_name", "user_id"), &DiscordP2PMesh::GetUserDisplayName);
	ClassDB::bind_method(D_METHOD("get_discord_sdk_version"), &DiscordP2PMesh::GetDiscordSdkVersion);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "application_id"), "set_application_id", "get_application_id");
	ADD_GROUP("ICE Servers", "");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "stun_server_host"), "set_stun_server_host", "get_stun_server_host");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "stun_server_port"), "set_stun_server_port", "get_stun_server_port");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "turn_server_host"), "set_turn_server_host", "get_turn_server_host");
	ADD_PROPERTY(PropertyInfo(Variant::INT, "turn_server_port"), "set_turn_server_port", "get_turn_server_port");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "turn_username"), "set_turn_username", "get_turn_username");
	ADD_PROPERTY(PropertyInfo(Variant::STRING, "turn_password"), "set_turn_password", "get_turn_password");

	ADD_SIGNAL(MethodInfo("discord_ready"));
	ADD_SIGNAL(MethodInfo("discord_auth_failed", PropertyInfo(Variant::STRING, "message")));
	ADD_SIGNAL(MethodInfo("discord_status_changed",
			PropertyInfo(Variant::INT, "status"),
			PropertyInfo(Variant::INT, "error"),
			PropertyInfo(Variant::INT, "error_detail")));

	ADD_SIGNAL(MethodInfo("lobby_joined", PropertyInfo(Variant::INT, "lobby_id")));
	ADD_SIGNAL(MethodInfo("lobby_join_failed", PropertyInfo(Variant::STRING, "message")));
	ADD_SIGNAL(MethodInfo("lobby_left"));
	ADD_SIGNAL(MethodInfo("lobby_message_received",
			PropertyInfo(Variant::INT, "from_peer_id"),
			PropertyInfo(Variant::STRING, "text")));
	ADD_SIGNAL(MethodInfo("lobby_member_joined", PropertyInfo(Variant::INT, "member_id")));
	ADD_SIGNAL(MethodInfo("lobby_member_left", PropertyInfo(Variant::INT, "member_id")));

	ADD_SIGNAL(MethodInfo("peer_connected", PropertyInfo(Variant::INT, "peer_id")));
	ADD_SIGNAL(MethodInfo("peer_disconnected", PropertyInfo(Variant::INT, "peer_id")));
	ADD_SIGNAL(MethodInfo("peer_data_received",
			PropertyInfo(Variant::INT, "peer_id"),
			PropertyInfo(Variant::PACKED_BYTE_ARRAY, "data")));
}

void DiscordP2PMesh::SetApplicationId(int64_t application_id) {
	_application_id = application_id;
}

int64_t DiscordP2PMesh::GetApplicationId() const {
	return _application_id;
}

void DiscordP2PMesh::SetStunServerHost(const String &host) { _stun_server_host = host; }
String DiscordP2PMesh::GetStunServerHost() const { return _stun_server_host; }
void DiscordP2PMesh::SetStunServerPort(int64_t port) { _stun_server_port = port; }
int64_t DiscordP2PMesh::GetStunServerPort() const { return _stun_server_port; }

void DiscordP2PMesh::SetTurnServerHost(const String &host) { _turn_server_host = host; }
String DiscordP2PMesh::GetTurnServerHost() const { return _turn_server_host; }
void DiscordP2PMesh::SetTurnServerPort(int64_t port) { _turn_server_port = port; }
int64_t DiscordP2PMesh::GetTurnServerPort() const { return _turn_server_port; }
void DiscordP2PMesh::SetTurnUsername(const String &username) { _turn_username = username; }
String DiscordP2PMesh::GetTurnUsername() const { return _turn_username; }
void DiscordP2PMesh::SetTurnPassword(const String &password) { _turn_password = password; }
String DiscordP2PMesh::GetTurnPassword() const { return _turn_password; }

void DiscordP2PMesh::_process(double p_delta) {
	if (!_session) {
		return;
	}

	dpmesh_update(_session);

	DPMeshEvent event;
	while (dpmesh_poll_event(_session, &event)) {
		DispatchEvent(event);
	}
}

void DiscordP2PMesh::Login() {
	if (!_session) {
		if (_application_id == 0) {
			UtilityFunctions::push_error("DiscordP2PMesh.login(): application_id is not set. Set it to your Discord application's client ID from the Developer Portal.");
			return;
		}

		// Config strings are only read for the duration of dpmesh_create(); safe to use
		// temporaries here.
		CharString stun_host = _stun_server_host.utf8();
		CharString turn_host = _turn_server_host.utf8();
		CharString turn_user = _turn_username.utf8();
		CharString turn_pass = _turn_password.utf8();

		DPMeshConfig config = {};
		config.application_id = (uint64_t)_application_id;
		config.stun_server_host = _stun_server_host.is_empty() ? nullptr : stun_host.get_data();
		config.stun_server_port = (uint16_t)_stun_server_port;
		config.turn_server_host = _turn_server_host.is_empty() ? nullptr : turn_host.get_data();
		config.turn_server_port = (uint16_t)_turn_server_port;
		config.turn_username = turn_user.get_data();
		config.turn_password = turn_pass.get_data();

		_session = dpmesh_create(&config);
		set_process(true); // drives dpmesh_update() in _process()
	}

	dpmesh_login(_session);
}

bool DiscordP2PMesh::IsReady() const {
	return _session && dpmesh_is_ready(_session);
}

void DiscordP2PMesh::CreateOrJoinLobby(const String &secret) {
	if (!_session) {
		UtilityFunctions::push_error("DiscordP2PMesh.create_or_join_lobby(): call login() first.");
		return;
	}
	dpmesh_create_or_join_lobby(_session, secret.utf8().get_data());
}

void DiscordP2PMesh::LeaveLobby() {
	if (_session) {
		dpmesh_leave_lobby(_session);
	}
}

void DiscordP2PMesh::SendLobbyMessage(const String &text) {
	if (_session) {
		dpmesh_send_lobby_message(_session, text.utf8().get_data());
	}
}

void DiscordP2PMesh::SendToPeer(int64_t peer_id, const PackedByteArray &data) {
	if (_session) {
		dpmesh_send_to_peer(_session, peer_id, data.ptr(), (size_t)data.size());
	}
}

uint64_t DiscordP2PMesh::GetCurrentUserId() const {
	return _session ? dpmesh_get_current_user_id(_session) : 0;
}

String DiscordP2PMesh::GetUserDisplayName(int64_t user_id) const {
	if (!_session) {
		return String::num_int64(user_id);
	}
	return String(dpmesh_get_user_display_name(_session, (uint64_t)user_id));
}

String DiscordP2PMesh::GetDiscordSdkVersion() const {
	return String(dpmesh_get_discord_sdk_version());
}

void DiscordP2PMesh::DispatchEvent(const DPMeshEvent &event) {
	switch (event.type) {
		case DPMESH_EVENT_READY:
			emit_signal("discord_ready");
			break;
		case DPMESH_EVENT_AUTH_FAILED:
			emit_signal("discord_auth_failed", String(event.message.message));
			break;
		case DPMESH_EVENT_STATUS_CHANGED:
			emit_signal("discord_status_changed",
					event.status_changed.status, event.status_changed.error, event.status_changed.error_detail);
			break;
		case DPMESH_EVENT_LOBBY_JOINED:
			emit_signal("lobby_joined", (int64_t)event.lobby_joined.lobby_id);
			break;
		case DPMESH_EVENT_LOBBY_JOIN_FAILED:
			emit_signal("lobby_join_failed", String(event.message.message));
			break;
		case DPMESH_EVENT_LOBBY_LEFT:
			emit_signal("lobby_left");
			break;
		case DPMESH_EVENT_LOBBY_MESSAGE:
			emit_signal("lobby_message_received", (int64_t)event.lobby_message.from_user_id, String(event.lobby_message.text));
			break;
		case DPMESH_EVENT_LOBBY_MEMBER_JOINED:
			emit_signal("lobby_member_joined", (int64_t)event.lobby_member.member_id);
			break;
		case DPMESH_EVENT_LOBBY_MEMBER_LEFT:
			emit_signal("lobby_member_left", (int64_t)event.lobby_member.member_id);
			break;
		case DPMESH_EVENT_PEER_CONNECTED:
			emit_signal("peer_connected", event.peer_conn.peer_id);
			break;
		case DPMESH_EVENT_PEER_DISCONNECTED:
			emit_signal("peer_disconnected", event.peer_conn.peer_id);
			break;
		case DPMESH_EVENT_PEER_DATA: {
			PackedByteArray bytes;
			bytes.resize((int64_t)event.peer_data.size);
			if (event.peer_data.size > 0) {
				memcpy(bytes.ptrw(), event.peer_data.data, event.peer_data.size);
			}
			emit_signal("peer_data_received", event.peer_data.peer_id, bytes);
			break;
		}
		case DPMESH_EVENT_NONE:
			break;
	}
}
