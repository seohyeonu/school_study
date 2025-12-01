const express = require('express');
const router = express.Router({ mergeParams: true });
const commentController = require('../controllers/commentController');
const { authenticate } = require('../middlewares/authMiddleware');

router.post('/', authenticate, commentController.create);

router.delete('/:commentId', authenticate, commentController.remove);

module.exports = router;