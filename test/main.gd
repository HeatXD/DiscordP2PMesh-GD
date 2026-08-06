extends Control

var _known_peer_ids: Array = [] # everyone seen via lobby chat or a P2P connection, connected or not
var _connected_peer_ids: Dictionary = {} # peer_id (int) -> true
var _peer_histories: Dictionary = {} # peer_id (int) -> Array[String]
var _selected_peer_id: int = 0

@onready var _app_id_edit: LineEdit = %AppIdEdit
@onready var _connect_button: Button = %ConnectButton
@onready var _status_label: Label = %StatusLabel
@onready var _lobby_secret_edit: LineEdit = %LobbySecretEdit
@onready var _join_button: Button = %JoinButton
@onready var _leave_button: Button = %LeaveButton
@onready var _lobby_log: RichTextLabel = %LobbyLog
@onready var _lobby_message_edit: LineEdit = %LobbyMessageEdit
@onready var _send_lobby_button: Button = %SendLobbyButton
@onready var _peer_dropdown: OptionButton = %PeerDropdown
@onready var _p2p_log: RichTextLabel = %P2PLog
@onready var _peer_message_edit: LineEdit = %PeerMessageEdit
@onready var _send_p2p_button: Button = %SendP2PButton


func _ready() -> void:
	_connect_button.pressed.connect(_on_connect_pressed)
	_join_button.pressed.connect(_on_join_pressed)
	_leave_button.pressed.connect(_on_leave_pressed)
	_send_lobby_button.pressed.connect(_on_send_lobby_message_pressed)
	_lobby_message_edit.text_submitted.connect(func(_t): _on_send_lobby_message_pressed())
	_send_p2p_button.pressed.connect(_on_send_p2p_message_pressed)
	_peer_message_edit.text_submitted.connect(func(_t): _on_send_p2p_message_pressed())
	_peer_dropdown.item_selected.connect(_on_peer_selected)

	P2PMesh.discord_ready.connect(_on_discord_ready)
	P2PMesh.discord_auth_failed.connect(_on_discord_auth_failed)
	P2PMesh.lobby_joined.connect(_on_lobby_joined)
	P2PMesh.lobby_join_failed.connect(_on_lobby_join_failed)
	P2PMesh.lobby_left.connect(_on_lobby_left)
	P2PMesh.lobby_message_received.connect(_on_lobby_message_received)
	P2PMesh.lobby_member_joined.connect(_on_lobby_member_joined)
	P2PMesh.lobby_member_left.connect(_on_lobby_member_left)
	P2PMesh.peer_connected.connect(_on_peer_connected)
	P2PMesh.peer_disconnected.connect(_on_peer_disconnected)
	P2PMesh.peer_data_received.connect(_on_peer_data_received)

	_rebuild_peer_dropdown()


func _on_connect_pressed() -> void:
	var app_id := _app_id_edit.text.to_int()
	if app_id == 0:
		_set_status("Enter an application ID first.")
		return
	P2PMesh.application_id = app_id
	P2PMesh.login()
	_set_status("Logging in... approve in the Discord app or browser.")


func _on_discord_ready() -> void:
	_set_status("Connected as %s" % P2PMesh.get_user_display_name(P2PMesh.get_current_user_id()))


func _on_discord_auth_failed(message: String) -> void:
	_set_status("Auth failed: %s" % message)


func _on_join_pressed() -> void:
	var secret := _lobby_secret_edit.text
	if secret.is_empty():
		_set_status("Enter a lobby secret first.")
		return
	P2PMesh.create_or_join_lobby(secret)


func _on_leave_pressed() -> void:
	P2PMesh.leave_lobby()


func _on_lobby_joined(lobby_id: int) -> void:
	_set_status("Joined lobby %d" % lobby_id)


func _on_lobby_join_failed(message: String) -> void:
	_set_status("Lobby join failed: %s" % message)


func _on_lobby_left() -> void:
	_set_status("Left the lobby.")


func _on_send_lobby_message_pressed() -> void:
	var text := _lobby_message_edit.text
	if text.is_empty():
		return
	P2PMesh.send_lobby_message(text)
	_lobby_log.text += "You: %s\n" % text
	_lobby_message_edit.text = ""


func _on_lobby_message_received(from_peer_id: int, text: String) -> void:
	_add_known_peer(from_peer_id)
	_lobby_log.text += "%s: %s\n" % [P2PMesh.get_user_display_name(from_peer_id), text]


func _on_lobby_member_joined(member_id: int) -> void:
	_add_known_peer(member_id)
	_lobby_log.text += "-- %s joined the lobby --\n" % P2PMesh.get_user_display_name(member_id)


func _on_lobby_member_left(member_id: int) -> void:
	_lobby_log.text += "-- %s left the lobby --\n" % P2PMesh.get_user_display_name(member_id)


func _on_peer_connected(peer_id: int) -> void:
	_add_known_peer(peer_id)
	_connected_peer_ids[peer_id] = true
	_append_peer_history(peer_id, "[connected]")
	_rebuild_peer_dropdown()


func _on_peer_disconnected(peer_id: int) -> void:
	_connected_peer_ids.erase(peer_id)
	_append_peer_history(peer_id, "[disconnected]")
	_rebuild_peer_dropdown()


func _on_peer_data_received(peer_id: int, data: PackedByteArray) -> void:
	_append_peer_history(peer_id, "%s: %s" % [P2PMesh.get_user_display_name(peer_id), data.get_string_from_utf8()])


func _on_send_p2p_message_pressed() -> void:
	if _selected_peer_id == 0:
		_set_status("No peer selected yet.")
		return
	var text := _peer_message_edit.text
	if text.is_empty():
		return

	var was_connected: bool = _connected_peer_ids.has(_selected_peer_id)
	P2PMesh.send_to_peer(_selected_peer_id, text.to_utf8_buffer())
	_peer_message_edit.text = ""

	if was_connected:
		_append_peer_history(_selected_peer_id, "You: %s" % text)
	else:
		# First send to a peer only starts the connection; this message itself isn't delivered.
		_append_peer_history(_selected_peer_id, "[connecting...]")


func _on_peer_selected(index: int) -> void:
	_selected_peer_id = _known_peer_ids[index] if index >= 0 and index < _known_peer_ids.size() else 0
	_show_peer_history(_selected_peer_id)


func _add_known_peer(peer_id: int) -> void:
	if peer_id == 0 or peer_id == P2PMesh.get_current_user_id():
		return
	if _known_peer_ids.has(peer_id):
		return
	_known_peer_ids.append(peer_id)
	_peer_histories[peer_id] = []
	_rebuild_peer_dropdown()


func _rebuild_peer_dropdown() -> void:
	_peer_dropdown.clear()

	if _known_peer_ids.is_empty():
		_peer_dropdown.add_item("(no one seen yet, say something in lobby chat)")
		_peer_dropdown.disabled = true
		_on_peer_selected(-1)
		return

	_peer_dropdown.disabled = false
	for peer_id in _known_peer_ids:
		var label: String = P2PMesh.get_user_display_name(peer_id)
		if not _connected_peer_ids.has(peer_id):
			label += " (not connected)"
		_peer_dropdown.add_item(label)

	# Keep the current selection if it's still known, otherwise default to the first peer.
	var index: int = _known_peer_ids.find(_selected_peer_id)
	if index == -1:
		index = 0
	_peer_dropdown.select(index)
	_on_peer_selected(index)


func _append_peer_history(peer_id: int, line: String) -> void:
	var history: Array = _peer_histories.get(peer_id, [])
	history.append(line)
	_peer_histories[peer_id] = history
	if peer_id == _selected_peer_id:
		_p2p_log.text += line + "\n"


func _show_peer_history(peer_id: int) -> void:
	_p2p_log.text = ""
	for line in _peer_histories.get(peer_id, []):
		_p2p_log.text += line + "\n"


func _set_status(text: String) -> void:
	_status_label.text = text
