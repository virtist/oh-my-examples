package bowling

type Game struct {
	rolls     [21]int
	rollIndex int
}

func (g *Game) Roll(pins int) {
	g.rolls[g.rollIndex] = pins
	g.rollIndex++
}

func (g *Game) Score() int {
	score := 0
	rollIndex := 0

	for frameIndex := 0; frameIndex < 10; frameIndex++ {
		if g.isStrike(rollIndex) {
			score += g.strikeScore(rollIndex)
			rollIndex++
		} else if g.isSpare(rollIndex) {
			score += g.spareScore(rollIndex)
			rollIndex += 2
		} else {
			score += g.frameScore(rollIndex)
			rollIndex += 2
		}
	}
	return score
}

func (g *Game) isStrike(rollIndex int) bool {
	return g.rolls[rollIndex] == 10
}

func (g *Game) isSpare(rollIndex int) bool {
	return g.rolls[rollIndex]+g.rolls[rollIndex+1] == 10
}

func (g *Game) strikeScore(rollIndex int) int {
	return 10 + g.rolls[rollIndex+1] + g.rolls[rollIndex+2]
}

func (g *Game) spareScore(rollIndex int) int {
	return 10 + g.rolls[rollIndex+2]
}

func (g *Game) frameScore(rollIndex int) int {
	return g.rolls[rollIndex] + g.rolls[rollIndex+1]
}
