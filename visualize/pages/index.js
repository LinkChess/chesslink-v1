import React, { useEffect, useState } from 'react';

// ... keep getPieceSymbol and parseFenToBoard as-is ...
function getPieceSymbol(piece) {
  const pieceSymbols = {
    'K': '♔', 'Q': '♕', 'R': '♖', 'B': '♗', 'N': '♘', 'P': '♙',
    'k': '♚', 'q': '♛', 'r': '♜', 'b': '♝', 'n': '♞', 'p': '♟'
  };
  return pieceSymbols[piece] || '';
}

// Parse a FEN string into a 2D board array (no validation)
function parseFenToBoard(fen) {
  const board = Array(8).fill(null).map(() => Array(8).fill(''));
  
  // Handle undefined or empty FEN strings
  if (!fen) {
    return board;
  }

  try {
    const rows = fen.split('/');
    for (let i = 0; i < Math.min(rows.length, 8); i++) {
      let colIndex = 0;
      for (const char of rows[i]) {
        if (!isNaN(char)) {
          colIndex += parseInt(char, 10);
        } else if (colIndex < 8) {
          board[i][colIndex++] = char;
        }
      }
    }
  } catch (error) {
    console.error('Error parsing FEN:', error);
  }

  return board;
}

export default function Home({ fenString: initialFen }) {
  // Default FEN string for starting chess position if none is provided
  const defaultFen = 'rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1';
  const [fenString, setFenString] = useState(initialFen || defaultFen);
  const [board, setBoard] = useState(parseFenToBoard(initialFen || defaultFen));

  useEffect(() => {
    const interval = setInterval(async () => {
      try {
        const res = await fetch('/api/fen');
        console.log('Fetched FEN data');
        const data = await res.json();

        if (data.fen && data.fen !== fenString) {
          setFenString(data.fen);
          setBoard(parseFenToBoard(data.fen));
        }
      } catch (err) {
        console.error('Failed to fetch FEN:', err);
      }
    }, 500); // 500ms interval

    return () => clearInterval(interval); // cleanup
  }, [fenString]);

  return (
    <div className="container">
      <h1>Chess Board Visualizer</h1>
      <div className="board">
        {board.map((row, rowIndex) => (
          <div className="row" key={rowIndex}>
            {row.map((square, colIndex) => {
              const isLight = (rowIndex + colIndex) % 2 === 0;
              return (
                <div
                  key={colIndex}
                  className={`square ${isLight ? 'light' : 'dark'}`}
                >
                  {getPieceSymbol(square)}
                </div>
              );
            })}
          </div>
        ))}
      </div>
      <div className="fen-string">
        <p>FEN: {fenString || 'No FEN found or file missing.'}</p>
      </div>

      {/* Styles unchanged */}
      <style jsx>{`
        .container {
          margin: 40px auto;
          display: flex;
          flex-direction: column;
          align-items: center;
          font-family: Arial, sans-serif;
        }

        .board {
          display: inline-block;
          border: 4px solid #333;
          box-shadow: 0 4px 8px rgba(0, 0, 0, 0.2);
        }

        .row {
          display: flex;
        }

        .square {
          width: 70px;
          height: 70px;
          font-size: 42px;
          display: flex;
          justify-content: center;
          align-items: center;
        }

        .light {
          background-color: #f0d9b5;
        }

        .dark {
          background-color: #b58863;
        }

        .fen-string {
          margin-top: 30px;
          font-family: monospace;
          word-break: break-word;
          text-align: center;
        }
      `}</style>
    </div>
  );
}
