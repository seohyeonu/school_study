const express = require('express');
const router = express.Router();
const postController = require('../controllers/postController');
const { authenticate } = require('../middlewares/authMiddleware');
const likeController = require('../controllers/likeController');

router.get('/', postController.list);

router.post('/', authenticate, postController.create);

router.get('/:id', postController.detail);

router.get('/:postId/like-status', authenticate, likeController.status);

router.delete('/:id', authenticate, postController.remove);

module.exports = router;