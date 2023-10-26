package bowling_test

import (
	"bowling"
	"testing"

	"github.com/stretchr/testify/assert"
)

func TestGutterGame(t *testing.T) {
	game := bowling.Game{}
	rollMany(&game, 20, 0)
	assert.Equal(t, 0, game.Score())
}

func TestAllOnes(t *testing.T) {
	game := bowling.Game{}
	rollMany(&game, 20, 1)
	assert.Equal(t, 20, game.Score())
}

func TestOneSpare(t *testing.T) {
	game := bowling.Game{}
	game.Roll(5)
	game.Roll(5)
	game.Roll(3)
	rollMany(&game, 17, 0)
	assert.Equal(t, 16, game.Score())
}

func TestOneStrike(t *testing.T) {
	game := bowling.Game{}
	game.Roll(10)
	game.Roll(4)
	game.Roll(3)
	rollMany(&game, 16, 0)
	assert.Equal(t, 24, game.Score())
}

func TestPerfectGame(t *testing.T) {
	game := bowling.Game{}
	rollMany(&game, 20, 10)
	assert.Equal(t, 300, game.Score())
}

func rollMany(g *bowling.Game, n, pins int) {
	for i := 0; i < n; i++ {
		g.Roll(pins)
	}
}
