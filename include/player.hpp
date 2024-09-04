#include <globals.hpp>
#include <networking.hpp>


class Player {
public:
    PlayerState State();
    void PollInput();
    void Update();
    Connection* GetConnection();
    void Draw();
    Vector2 _position;
private:
    void PollConnection();
    void ParseReceivedData();
    PlayerState _state;
    int8_t _requested_state;
    Connection _conn;
    Color color;
};