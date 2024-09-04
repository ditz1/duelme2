#include <player.hpp>

Player::Player() {
    _requested_state = 0;
    _state = IDLE;
    _conn.connected = false;
    _position = {400, 225};
}

Player::~Player() {
    CloseWebSocket(_conn.ws);
}

Connection* Player::GetConnection() {
    return &_conn;
}

void Player::PollInput() {
    if (IsKeyDown(KEY_D)){
        _requested_state = 1;
        return;
    } else if (IsKeyDown(KEY_A)){
        _requested_state = 2;
        return;
    } else if (IsKeyDown(KEY_W)){
        _requested_state = 3;
        return;
    } else if (IsKeyDown(KEY_S)){
        _requested_state = 4;
        return;
    } else {
        _requested_state = 0;
    }
}

PlayerState Player::State() {
    return _state;
}

PlayerState Player::RequestedState() {
    return (PlayerState)_requested_state;
}


void Player::PollConnection() {
    if (_conn.last_received->numBytes > 0) {
        std::cout << "Received message of size: " << (int)_conn.last_received->numBytes << std::endl;
        for (int i = 0; i < _conn.last_received->numBytes; i++) {
            printf("0x%x | ", _conn.last_received->data[i]);
        }
        printf("\n");
    }
}

void Player::Draw() {
    DrawCircleV(_position, 20, RED);
}

int Player::Id() {
    return _id;
}

void Player::ParseReceivedData() {
    int state = _conn.last_received->data[0];
    if (_conn.last_received->numBytes > 1) {
        std::cout << "Received more than one byte" << std::endl;
        return;
    }
    if (state != _state){
        //std::cout << "PlayerState changed from " << PlayerStateToString.at((PlayerState)_state) << " to " << PlayerStateToString.at((PlayerState)state) << std::endl;
    }
    switch (state) {
        case 0:
            _state = IDLE;
            break;
        case 1:
            _state = MOVE_RIGHT;
            break;
        case 2:
            _state = MOVE_LEFT;
            break;
        case 3:
            _state = MOVE_UP;
            break;
        case 4:
            _state = MOVE_DOWN;
            break;
        default:
            _state = IDLE;
            //std::cout << "Invalid PlayerState received" << std::endl;
            break;
    }
}

void Player::Update() {
    PollConnection();

    ParseReceivedData();

    PollInput();
    if (_state != _requested_state) {
        ClientSendBytes(_conn.ws, (void*)&_requested_state, 1);
    }
    //ClientSendBytes(_conn.ws, (void*)&_requested_state, 1);

    switch (_state) {
        case IDLE:
            break;
        case MOVE_RIGHT:
            _position.x += 1;
            break;
        case MOVE_LEFT:
            _position.x -= 1;
            break;
        case MOVE_DOWN:
            _position.y += 1;
            break;
        case MOVE_UP:
            _position.y -= 1;
            break;
    }
    _conn.last_received->numBytes = 0;

}