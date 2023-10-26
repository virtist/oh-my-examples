import unittest
import bowling_game as bowling


class TestBowlingGame(unittest.TestCase):

    def setUp(self):
        self.game = bowling.BowlingGame()

    def testGutterGame(self):
        self._rollMany(20, 0)
        assert self.game.score() == 0

    def testAllOnes(self):
        self._rollMany(20, 1)
        assert self.game.score() == 20

    def testOneSpare(self):
        self.game.roll(5)
        self.game.roll(5)
        self.game.roll(3)
        self._rollMany(17, 0)
        assert self.game.score() == 16

    def testOneStrike(self):
        self.game.roll(10)
        self.game.roll(4)
        self.game.roll(3)
        self._rollMany(16, 0)
        assert self.game.score() == 24

    def testPerfectGame(self):
        self._rollMany(12, 10)
        assert self.game.score() == 300

    def testAllSpares(self):
        self._rollMany(21, 5)
        assert self.game.score() == 150
        
    def _rollMany(self, n, pins):
        for i in range(n):
            self.game.roll(pins)
