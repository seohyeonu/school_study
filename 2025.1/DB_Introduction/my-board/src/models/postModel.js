const pool = require('../config/db');

const Post = {
  async findAll({ offset, limit }) {
    const [rows] = await pool.execute(
      `
      SELECT
        p.id,
        p.title,
        p.user_id,
        u.username AS author,
        p.created_at,
        IFNULL(c.comment_count, 0) AS comment_count,
        IFNULL(l.like_count, 0) AS like_count
      FROM posts p
      JOIN users u ON p.user_id = u.id
      LEFT JOIN (
        SELECT post_id, COUNT(*) AS comment_count
        FROM comments
        GROUP BY post_id
      ) c ON p.id = c.post_id
      LEFT JOIN (
        SELECT post_id, COUNT(*) AS like_count
        FROM likes
        GROUP BY post_id
      ) l ON p.id = l.post_id
      ORDER BY p.created_at DESC
      LIMIT ?, ?
      `,
      [offset, limit]
    );
    return rows;
  },

  async countAll() {
    const [rows] = await pool.execute(`SELECT COUNT(*) AS total FROM posts`);
    return rows[0].total;
  },

  async findById(postId) {
    const [rows] = await pool.execute(
      `
      SELECT
        p.*,
        u.username AS author
      FROM posts p
      JOIN users u ON p.user_id = u.id
      WHERE p.id = ?
      `,
      [postId]
    );
    return rows[0] || null;
  },

  // 게시글 생성
  async create({ userId, title, content }) {
    const [result] = await pool.execute(
      `INSERT INTO posts (user_id, title, content) VALUES (?, ?, ?)`,
      [userId, title, content]
    );
    return result.insertId;
  },

  async delete(postId) {
    await pool.execute(`DELETE FROM posts WHERE id = ?`, [postId]);
    return;
  }
};

module.exports = Post;