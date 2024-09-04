import asyncio
import websockets

global num_connections

async def handle_connection(websocket, path):
    try:
        print("New connection")
        num_connections += 1
        #send connected id back to player
        await websocket.send(f"connected: {num_connections}")
        async for message in websocket:
            print(f"Received message: {message}")
            await websocket.send(message)  # Echoes the received message
            if (websocket.closed):
                print("Connection closed")
                break
    except websockets.exceptions.ConnectionClosed as e:
        print(f"Connection closed: {e}")

async def main():
    server = await websockets.serve(handle_connection, "192.168.1.42", 9000)
    print("Server started...")
    await server.wait_closed()

if __name__ == "__main__":
    asyncio.run(main())
