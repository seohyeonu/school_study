const pool = require('../config/db');

const Comment = {
  async findByPostId(postId) {
    const [rows] = await pool.execute(
      `
      SELECT c.id, c.post_id, c.user_id, u.username AS author, c.content, c.created_at
      FROM comments c
      JOIN users u ON c.user_id = u.id
      WHERE c.post_id = ?
      ORDER BY c.created_at ASC
      `,
      [postId]
    );
    return rows;
  },

  async create({ postId, userId, content }) {
    const [result] = await pool.execute(
      `INSERT INTO comments (post_id, user_id, content) VALUES (?, ?, ?)`,
      [postId, userId, content]
    );
    return result.insertId;
  },

  async delete(commentId) {
    await pool.execute(`DELETE FROM comments WHERE id = ?`, [commentId]);
    return;
  },

  async findById(commentId) {
    const [rows] = await pool.execute(
      `SELECT * FROM comments WHERE id = ?`,
      [commentId]
    );
    return rows[0] || null;
  }
};

module.exports = Comment;