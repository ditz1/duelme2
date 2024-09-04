import asyncio
import websockets

num_connections = 1
async def handle_connection(websocket, path):
    global num_connections
    try:
        print("New connection")
        num_connections += 1
        #send connected id back to player
        await websocket.send(f"{num_connections}")
        async for message in websocket:
            print(f"Received message: {message}")
            await websocket.send(message)  # Echoes the received message
            if (websocket.closed):
                print("Connection closed")
                break
    except websockets.exceptions.ConnectionClosed as e:
        print(f"Connection closed: {e}")
        num_connections -= 1

async def main():
    server = await websockets.serve(handle_connection, "192.168.1.42", 9000)
    print("Server started...")
    await server.wait_closed()
    

if __name__ == "__main__":
    asyncio.run(main())
