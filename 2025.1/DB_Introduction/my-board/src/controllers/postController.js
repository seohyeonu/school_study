// src/controllers/postController.js
const Post = require('../models/postModel');
const Comment = require('../models/commentModel'); // 댓글 조회용
const Like = require('../models/likeModel');       // 좋아요 수 관리용

module.exports = {
  // (1) 전체 게시글 목록 조회
  async list(req, res, next) {
    try {
      const page = parseInt(req.query.page) || 1;
      const limit = parseInt(req.query.limit) || 10; // 기본 10개
      const offset = (page - 1) * limit;

      const total = await Post.countAll();
      const posts = await Post.findAll({ offset, limit });

      return res.json({
        page,
        limit,
        total,
        posts
      });
    } catch (err) {
      next(err);
    }
  },

  // (2) 게시글 상세 조회 (본문 + 댓글)
  async detail(req, res, next) {
    try {
      const postId = parseInt(req.params.id);
      const post = await Post.findById(postId);
      if (!post) {
        return res.status(404).json({ message: '존재하지 않는 게시글입니다.' });
      }
      const comments = await Comment.findByPostId(postId);
      return res.json({ post, comments });
    } catch (err) {
      next(err);
    }
  },

  async create(req, res, next) {
    try {
      const { title, content } = req.body;
      if (!title || !content) {
        return res.status(400).json({ message: '제목과 내용을 입력해주세요.' });
      }
      const userId = req.user.id;
      const newPostId = await Post.create({ userId, title, content });
      return res.status(201).json({ postId: newPostId });
    } catch (err) {
      next(err);
    }
  },

  async remove(req, res, next) {
    try {
      const postId = parseInt(req.params.id);
      const userId = req.user.id;
      const post = await Post.findById(postId);
      if (!post) {
        return res.status(404).json({ message: '존재하지 않는 게시글입니다.' });
      }
      if (post.user_id !== userId) {
        return res.status(403).json({ message: '권한이 없습니다.' });
      }
      await Post.delete(postId);
      return res.json({ message: '삭제되었습니다.' });
    } catch (err) {
      next(err);
    }
  }
};