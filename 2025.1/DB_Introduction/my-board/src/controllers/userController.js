// src/controllers/userController.js
const User = require('../models/userModel');
const bcrypt = require('bcrypt');
const jwt = require('jsonwebtoken');

module.exports = {
  async register(req, res, next) {
    try {
      const { username, password, name, email } = req.body;
      if (!username || !password || !name) {
        return res.status(400).json({ message: '필수 항목이 누락되었습니다.' });
      }
      const existing = await User.findByUsername(username);
      if (existing) {
        return res.status(409).json({ message: '이미 사용 중인 아이디입니다.' });
      }
      const newUserId = await User.create({ username, password, name, email });
      return res.status(201).json({ userId: newUserId });
    } catch (err) {
      next(err);
    }
  },

  async login(req, res, next) {
    try {
      const { username, password } = req.body;
      if (!username || !password) {
        return res.status(400).json({ message: '아이디와 비밀번호를 입력해주세요.' });
      }
      const user = await User.findByUsername(username);
      if (!user) {
        return res.status(401).json({ message: '존재하지 않는 사용자입니다.' });
      }
      const match = await bcrypt.compare(password, user.password);
      if (!match) {
        return res.status(401).json({ message: '비밀번호가 올바르지 않습니다.' });
      }
      const payload = { id: user.id, username: user.username };
      const token = jwt.sign(payload, process.env.JWT_SECRET, {
        expiresIn: '6h'
      });
      return res.json({ token, user: { id: user.id, username: user.username, name: user.name } });
    } catch (err) {
      next(err);
    }
  }
};