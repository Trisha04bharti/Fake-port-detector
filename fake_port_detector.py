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