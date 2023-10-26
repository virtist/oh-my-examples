class BowlingGame:
    def __init__(self) -> None:
        self.rolls = []

    def roll(self, pins):
        self.rolls.append(pins)

    def score(self):
        result = 0
        rollIndex = 0
        for frameIndex in range(10):
            if self._isStrike(rollIndex):
                result += self._strikeScore(rollIndex)
                rollIndex += 1
            elif self._isSpare(rollIndex):
                result += self._spareScore(rollIndex)
                rollIndex += 2
            else:
                result += self._frameScore(rollIndex)
                rollIndex += 2
        return result

    def _isStrike(self, rollIndex):
        return self.rolls[rollIndex] == 10

    def _isSpare(self, rollIndex):
        return self.rolls[rollIndex] + self.rolls[rollIndex+1] == 10

    def _strikeScore(self, rollIndex):
        return 10 + self.rolls[rollIndex+1] + self.rolls[rollIndex+2]

    def _spareScore(self, rollIndex):
        return 10 + self.rolls[rollIndex+2]

    def _frameScore(self, rollIndex):
        return self.rolls[rollIndex] + self.rolls[rollIndex+1]
