#include <globals.hpp>
#include <networking.hpp>

class Player {
public:
    Player();
    ~Player();
    PlayerState State();
    PlayerState RequestedState();
    void PollInput();
    void Update();
    Connection* GetConnection();
    void Draw();
    int Id();
    Vector2 Position();
private:
    void PollConnection();
    void ParseReceivedData();
    int _id;
    Vector2 _position;
    PlayerState _state;
    int8_t _requested_state;
    Connection _conn;
    Color color;
};