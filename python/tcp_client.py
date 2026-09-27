import socket

SERVER_IP = "169.254.228.251"
SERVER_PORT = 5000

message = b"HELLO STM32"

print("Connecting to STM32...")
print(f"Server: {SERVER_IP}:{SERVER_PORT}")

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client:
    client.connect((SERVER_IP, SERVER_PORT))
    print("Connected to STM32 W5500!")

    print(f"Sending: {message.decode()}")
    client.sendall(message)

    response = client.recv(256)
    print(f"Received: {response.decode(errors='replace')}")

print("Connection closed.")
