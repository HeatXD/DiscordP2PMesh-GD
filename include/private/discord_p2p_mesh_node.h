#ifndef DPMESH_GD_DISCORD_P2P_MESH_NODE_H
#define DPMESH_GD_DISCORD_P2P_MESH_NODE_H

#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/variant/packed_byte_array.hpp>

#include <discord_p2p_mesh.h>

namespace godot {

// Thin adapter: forwards to the dpmesh_* C API and turns polled DPMeshEvents into signals,
// owning no P2P/Discord logic of its own, that all lives in DiscordP2PMesh (the core library).
class DiscordP2PMesh : public Node {
	GDCLASS(DiscordP2PMesh, Node)

protected:
	static void _bind_methods();

public:
	DiscordP2PMesh();
	~DiscordP2PMesh() override;

	void _process(double p_delta) override;

	void SetApplicationId(int64_t application_id);
	int64_t GetApplicationId() const;

	void SetStunServerHost(const String &host);
	String GetStunServerHost() const;
	void SetStunServerPort(int64_t port);
	int64_t GetStunServerPort() const;

	void SetTurnServerHost(const String &host);
	String GetTurnServerHost() const;
	void SetTurnServerPort(int64_t port);
	int64_t GetTurnServerPort() const;
	void SetTurnUsername(const String &username);
	String GetTurnUsername() const;
	void SetTurnPassword(const String &password);
	String GetTurnPassword() const;

	void Login();
	bool IsReady() const;

	void CreateOrJoinLobby(const String &secret);
	void LeaveLobby();
	void SendLobbyMessage(const String &text);
	void SendToPeer(int64_t peer_id, const PackedByteArray &data);

	uint64_t GetCurrentUserId() const;
	String GetUserDisplayName(int64_t user_id) const;

	String GetDiscordSdkVersion() const;

private:
	void DispatchEvent(const DPMeshEvent &event);

	DPMeshSession *_session = nullptr;

	int64_t _application_id = 0;
	String _stun_server_host = "stun.l.google.com";
	int64_t _stun_server_port = 19302;
	String _turn_server_host;
	int64_t _turn_server_port = 0;
	String _turn_username;
	String _turn_password;
};

} // namespace godot

#endif // DPMESH_GD_DISCORD_P2P_MESH_NODE_H
