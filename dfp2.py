import psutil
import socket

# Initially known legitimate ports
LEGITIMATE_PORTS = {22, 80, 443, 3306, 8080}

def get_open_ports():
    """Returns a list of open ports on the system"""
    open_ports = set()
    for conn in psutil.net_connections(kind='inet'):
        if conn.status == 'LISTEN':
            open_ports.add(conn.laddr.port)
    return open_ports

def detect_fake_ports():
    """Detects unauthorized ports and allows dynamic approval"""
    open_ports = get_open_ports()
    fake_ports = open_ports - LEGITIMATE_PORTS  

    if fake_ports:
        print(f"[!] Detected suspicious ports: {fake_ports}")
        for port in fake_ports:
            user_input = input(f"Is port {port} legitimate? (yes/no): ").strip().lower()
            if user_input == "yes":
                LEGITIMATE_PORTS.add(port)  # Dynamically add as legitimate
                print(f"[+] Port {port} added to legitimate ports.")
            else:
                print(f"[!] Port {port} remains classified as suspicious.")
    else:
        print("[+] No fake ports detected. System is secure.")

if __name__ == "__main__":
    detect_fake_ports()
