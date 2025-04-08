import socket
import threading

# List of fake ports to open
FAKE_PORTS = [4444, 5555, 6666]

def open_fake_port(port):
    """Function to open a fake port and keep it listening"""
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind(("0.0.0.0", port))
    s.listen(5)
    print(f"[+] Fake port {port} is open and listening...")
    while True:
        conn, addr = s.accept()
        print(f"[*] Connection received on fake port {port} from {addr}")
        conn.close()

# Create a thread for each fake port
for port in FAKE_PORTS:
    thread = threading.Thread(target=open_fake_port, args=(port,))
    thread.daemon = True
    thread.start()

# Keep script running
try:
    while True:
        pass
except KeyboardInterrupt:
    print("\n[!] Stopping fake ports simulation...")
