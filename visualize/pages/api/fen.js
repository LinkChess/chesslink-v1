// pages/api/fen.js
import fs from 'fs';
import path from 'path';

export default function handler(req, res) {
  const fenPath = path.join(process.cwd(), '..', 'app', 'current_fen.txt');

  try {
    const data = fs.readFileSync(fenPath, 'utf8');
    const lines = data.split('\n').filter(line => line.trim() !== '');
    const fen = lines[lines.length - 1] || '';
    res.status(200).json({ fen });
  } catch (err) {
    console.error('Error reading FEN:', err);
    res.status(500).json({ fen: '' });
  }
}
