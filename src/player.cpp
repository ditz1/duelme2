#include <player.hpp>


Connection* Player::GetConnection() {
    return &_conn;
}

void Player::PollInput() {
    _requested_state = 0; // reset
    if (IsKeyPressed(KEY_ONE)){
        _requested_state = 1;
    }
    if (IsKeyPressed(KEY_TWO)){
        _requested_state = 2;
    }
    if (IsKeyPressed(KEY_THREE)){
        _requested_state = 3;
    }
    if (IsKeyPressed(KEY_FOUR)){
        _requested_state = 4;
    }
}


void Player::PollConnection() {
    if (_conn.last_received->numBytes > 0) {
        std::cout << "Received message of size: " << _conn.last_received->numBytes << std::endl;
        for (int i = 0; i < _conn.last_received->numBytes; i++) {
            printf("0x%x\n", _conn.last_received->data[i]);
        }
        _conn.last_received->numBytes = 0;
    }
}

void Player::Draw() {
    DrawCircleV(_position, 20, RED);
}

void Player::ParseReceivedData() {
    int state = _conn.last_received->data[0];
    switch (state) {
        case 0:
            _state = PlayerState::IDLE;
            break;
        case 1:
            _state = PlayerState::MOVE_RIGHT;
            break;
        case 2:
            _state = PlayerState::MOVE_LEFT;
            break;
        case 3:
            _state = PlayerState::MOVE_UP;
            break;
        case 4:
            _state = PlayerState::MOVE_DOWN;
            break;
    }
}

void Player::Update() {
    PollInput();
    if (_requested_state < 0) {
        ClientSendBytes(_conn.ws, (void*)&_requested_state, 1);
        _requested_state = 0;
    } else {
        return;
    }
    ClientSendBytes(_conn.ws, (void*)&_requested_state, 1);
    PollConnection();
    ParseReceivedData();
    switch (_state) {
        case PlayerState::IDLE:
            break;
        case PlayerState::MOVE_RIGHT:
            _position.x += 1;
            break;
        case PlayerState::MOVE_LEFT:
            _position.x -= 1;
            break;
        case PlayerState::MOVE_DOWN:
            _position.y += 1;
            break;
        case PlayerState::MOVE_UP:
            _position.y -= 1;
            break;
    }
}