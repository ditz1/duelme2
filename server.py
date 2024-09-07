import asyncio
import websockets

connected_clients = set()

## protocol ##
CONNECT = b'\xA1'
DISCONNECT = b'\xA2'
AUTH_GAMESTATE = b'\xA3'
REQ_GAMESTATE = b'\xA4'

async def handle_connection(websocket, path):
    global num_connections
    try:
        print(f"New connection from {websocket.remote_address}")
        connected_clients.add(websocket)
        client_id = len(connected_clients)
        await websocket.send(f"{client_id}")
        
        print(f"Number of connected clients: {len(connected_clients)}")
        
        async for message in websocket:
            print(f"Received message: {message}")
            await websocket.send(message) 
            
        if websocket.closed:
            print(f"Connection closed: {websocket.remote_address}")
    except websockets.exceptions.ConnectionClosed as e:
        print(f"Connection closed: {e}")
    finally:
        connected_clients.remove(websocket)
        print(f"Number of connected clients: {len(connected_clients)}")

async def send_bytes(websocket, message: str):
    await websocket.send(message.encode())

async def broadcast(message: str):
    for client in connected_clients:
        await send_bytes(client, message)

async def list_clients():
    for i, client in enumerate(connected_clients, 1):
        print(f"Client {i}: {client.remote_address}")
    print(f"Total connected clients: {len(connected_clients)}")

async def main():
    server = await websockets.serve(handle_connection, "192.168.1.42", 9000)
    print("Server started...")
    await server.wait_closed()
    

if __name__ == "__main__":
    asyncio.run(main())