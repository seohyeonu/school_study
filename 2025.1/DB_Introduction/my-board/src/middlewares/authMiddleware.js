// src/middlewares/authMiddleware.js
const jwt = require('jsonwebtoken');
const User = require('../models/userModel');

module.exports = {
  authenticate: async (req, res, next) => {
    const authHeader = req.headers.authorization;
    if (!authHeader || !authHeader.startsWith('Bearer ')) {
      return res.status(401).json({ message: '토큰이 없습니다.' });
    }
    const token = authHeader.split(' ')[1];
    try {
      const decoded = jwt.verify(token, process.env.JWT_SECRET);
      // 토큰에서 꺼낸 user id로 실제 사용자 존재 확인(optional)
      const user = await User.findById(decoded.id);
      if (!user) {
        return res.status(401).json({ message: '존재하지 않는 사용자입니다.' });
      }
      req.user = user; // req.user에 사용자 정보를 담아두면 이후 핸들러에서 사용 가능
      next();
    } catch (err) {
      return res.status(401).json({ message: '유효하지 않은 토큰입니다.' });
    }
  }
};