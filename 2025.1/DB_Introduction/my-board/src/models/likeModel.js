const pool = require('../config/db');

const Like = {
  async findOne({ postId, userId }) {
    const [rows] = await pool.execute(
      `SELECT * FROM likes WHERE post_id = ? AND user_id = ? LIMIT 1`,
      [postId, userId]
    );
    return rows[0] || null;
  },

  async create({ postId, userId }) {
    const [result] = await pool.execute(
      `INSERT INTO likes (post_id, user_id) VALUES (?, ?)`,
      [postId, userId]
    );
    return result.insertId;
  },


  async delete({ postId, userId }) {
    await pool.execute(
      `DELETE FROM likes WHERE post_id = ? AND user_id = ?`,
      [postId, userId]
    );
    return;
  },
  async countByPostId(postId) {
    const [rows] = await pool.execute(
      `SELECT COUNT(*) AS like_count FROM likes WHERE post_id = ?`,
      [postId]
    );
    return rows[0].like_count;
  }
};

module.exports = Like;