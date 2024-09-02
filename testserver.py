import asyncio
import websockets

async def handle_connection(websocket, path):
    try:
        async for message in websocket:
            print(f"Received message: {message}")
            await websocket.send(message)  # Echoes the received message
            if (websocket.closed):
                print("Connection closed")
                break
    except websockets.exceptions.ConnectionClosed as e:
        print(f"Connection closed: {e}")

async def main():
    server = await websockets.serve(handle_connection, "localhost", 9000)
    print("Server started...")
    await server.wait_closed()

if __name__ == "__main__":
    asyncio.run(main())
