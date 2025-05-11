import serial
import threading
import json
import time

board_state = {}

# Define mappings for each quadrant board to standard chess coordinates
# Each key is a position on the quadrant board, and value is the actual chess square
BOARD_MAPPINGS = {
    'A': ['h1', 'g1', 'f1', 'e1', 'e2', 'f2', 'g2', 'h2', 'h3', 'g3', 'f3', 'e3', 'e4', 'f4', 'g4', 'h4'],
    'B': ['h5', 'g5', 'f5', 'e5', 'e6', 'f6', 'g6', 'h6', 'h7', 'g7', 'f7', 'e7', 'e8', 'f8', 'g8', 'h8'],
    'C': ['a8', 'b8', 'c8', 'd8', 'd7', 'c7', 'b7', 'a7', 'a6', 'b6', 'c6', 'd6', 'd5', 'c5', 'b5', 'a5'],
    'D': ['a4', 'b4', 'c4', 'd4', 'd3', 'c3', 'b3', 'a3', 'a2', 'b2', 'c2', 'd2', 'd1', 'c1', 'b1', 'a1']
}

def is_json(line: str) -> bool:
    return line.startswith("{") and line.endswith("}")

def read_from_port(region, port):
    ser = serial.Serial(port, 9600)
    while True:
        try:
            line = ser.readline().decode('utf-8').strip()

            # if not is_json(line):
            #     print(f"[{region}][DEBUG] {line}")
            #     continue

            if ":" in line:
                parts = line.split(":", 1)
                board_id = parts[0].replace("board", "")
                # data = {"squares": {parts[1]}}
                if len(parts) > 1:
                    # board_state_str = parts[1].strip()
                    # Further processing of the board state string would go here
                    # print(f"[{region}][INFO] Received board {board_id}: {board_state_str}")
                    
                    for i in range(len(parts[1])):
                        chess_square = BOARD_MAPPINGS[region][i]
                        board_state[chess_square] = parts[1][i]
                        # print(f"[{region}][INFO] {i}: {BOARD_MAPPINGS[region][i]} = {parts[1][i]}")
                        # print(f"[{region}][INFO] Board state: {board_state}")
            else:
                # Fallback to JSON parsing for other message types
                try:
                    data = json.loads(line)
                except json.JSONDecodeError:
                    print(f"[{region}][ERROR] Invalid format: {line}")
                    continue
                
                
        except Exception as e:
            print(f"[{region}][ERROR] Failed to parse: {e}")

def fen_from_board():
    full_rows = []
    for rank in range(8, 0, -1):
        row = ''
        empty = 0
        for file in 'abcdefgh':
            sq = f"{file}{rank}"
            piece = board_state.get(sq, '.')
            if piece == '.' or piece == '-':  # Consider both '.' and '-' as empty squares
                empty += 1
            else:
                if empty > 0:
                    row += str(empty)
                    empty = 0
                row += piece
        if empty > 0:
            row += str(empty)
        full_rows.append(row)
    return "/".join(full_rows)

# Map regions to ports
ports = {
    'A': '/dev/cu.usbserial-1130',
    'B': '/dev/cu.usbserial-1110',
    'C': '/dev/cu.usbserial-110',
    'D': '/dev/cu.usbserial-130',
}

# Define file path to save FEN
fen_file_path = "/Users/victorzheng/Documents/classes/chesslink/hardware/pcb/v1/app/current_fen.txt"

# Variable to track the last FEN value
last_fen = ""

# Start a thread for each serial device
for region, port in ports.items():
    t = threading.Thread(target=read_from_port, args=(region, port))
    t.daemon = True
    t.start()

# Print updated FEN every second and append to file only if there's a change
while True:
    time.sleep(1)
    current_fen = fen_from_board()
    print("Current FEN:", current_fen)
    
    # Only write to file if the FEN has changed
    if current_fen != last_fen:
        with open(fen_file_path, 'a') as f:
            f.write(current_fen + "\n")
        last_fen = current_fen

