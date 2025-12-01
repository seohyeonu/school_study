// src/controllers/likeController.js
const Like = require('../models/likeModel');

module.exports = {
  async toggle(req, res, next) {
    try {
      const postId = parseInt(req.params.postId);
      const userId = req.user.id;

      const existing = await Like.findOne({ postId, userId });
      if (existing) {
        await Like.delete({ postId, userId });
        return res.json({ liked: false });
      } else {
        await Like.create({ postId, userId });
        return res.json({ liked: true });
      }
    } catch (err) {
      next(err);
    }
  },

  async status(req, res, next) {
    try {
      const postId = parseInt(req.params.postId);
      const userId = req.user.id;
      const existing = await Like.findOne({ postId, userId });
      return res.json({ liked: !!existing });
    } catch (err) {
      next(err);
    }
  },
};