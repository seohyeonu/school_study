const Comment = require('../models/commentModel');

module.exports = {
  async create(req, res, next) {
    try {
      const postId = parseInt(req.params.postId);
      const { content } = req.body;
      if (!content) {
        return res.status(400).json({ message: '내용을 입력해주세요.' });
      }
      const userId = req.user.id;
      const newCommentId = await Comment.create({ postId, userId, content });
      return res.status(201).json({ commentId: newCommentId });
    } catch (err) {
      next(err);
    }
  },

  async remove(req, res, next) {
    try {
      const commentId = parseInt(req.params.commentId);
      const userId = req.user.id;
      const comment = await Comment.findById(commentId);
      if (!comment) {
        return res.status(404).json({ message: '존재하지 않는 댓글입니다.' });
      }
      if (comment.user_id !== userId) {
        return res.status(403).json({ message: '권한이 없습니다.' });
      }
      await Comment.delete(commentId);
      return res.json({ message: '댓글이 삭제되었습니다.' });
    } catch (err) {
      next(err);
    }
  }
};