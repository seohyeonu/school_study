const express = require('express');
const router = express.Router({ mergeParams: true });
const likeController = require('../controllers/likeController');
const { authenticate } = require('../middlewares/authMiddleware');

router.post('/', authenticate, likeController.toggle);

module.exports = router;