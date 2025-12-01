const pool = require('../config/db');
const bcrypt = require('bcrypt');

const User = {
  async create({ username, password, name, email }) {
    const hashed = await bcrypt.hash(password, 10);
    const [result] = await pool.execute(
      `INSERT INTO users (username, password, name, email) VALUES (?, ?, ?, ?)`,
      [username, hashed, name, email]
    );
    return result.insertId;
  },

  async findByUsername(username) {
    const [rows] = await pool.execute(
      `SELECT * FROM users WHERE username = ? LIMIT 1`,
      [username]
    );
    return rows[0] || null;
  },

  async findById(id) {
    const [rows] = await pool.execute(
      `SELECT * FROM users WHERE id = ?`,
      [id]
    );
    return rows[0] || null;
  }
};

module.exports = User;