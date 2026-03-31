import socket
import sys
import os
import tty
import termios
import select
import struct
import fcntl
import signal

HOST = 'localhost'
PORT = 8080

def get_terminal_size():
    rows, cols, _, _ = struct.unpack('HHHH',
    fcntl.ioctl(sys.stdout.fileno(), termios.TIOCGWINSZ, b'\x00' * 8))
    return rows, cols

def main():
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((HOST, PORT))

    old_settings = termios.tcgetattr(sys.stdin)
    try:
        tty.setraw(sys.stdin.fileno())

        while True:
            rlist, _, _ = select.select([sys.stdin, sock], [], [])

            if sys.stdin in rlist:
                data = os.read(sys.stdin.fileno(), 1024)
                if not data:
                    break
                sock.sendall(data)

            if sock in rlist:
                data = sock.recv(1024)
                if not data:
                    break
                sys.stdout.buffer.write(data)
                sys.stdout.buffer.flush()

    finally:
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, old_settings)
        sock.close()

if __name__ == '__main__':
    main()
