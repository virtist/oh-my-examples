struct Game {
    rolls: [i32; 21],
    current_roll: usize,
}

impl Game {
    fn score(&self) -> i32 {
        let mut score = 0;
        let mut frame_index = 0;

        for _ in 0..10 {
            if self.is_spare(frame_index) {
                score += self.spare_bonus(frame_index);
                frame_index += 2;
            } else if self.is_strike(frame_index) {
                score += self.strike_bonus(frame_index);
                frame_index += 1;
            } else {
                score += self.sum_of_balls_in_frame(frame_index);
                frame_index += 2;
            }
        }
        return score;
    }

    fn spare_bonus(&self, frame_index: usize) -> i32 {
        10 + self.rolls[frame_index + 2]
    }

    fn strike_bonus(&self, frame_index: usize) -> i32 {
        10 + self.rolls[frame_index + 1] + self.rolls[frame_index + 2]
    }

    fn sum_of_balls_in_frame(&self, frame_index: usize) -> i32 {
        self.rolls[frame_index] + self.rolls[frame_index + 1]
    }

    fn is_strike(&self, frame_index: usize) -> bool {
        self.rolls[frame_index] == 10
    }

    fn roll(&mut self, pins: i32) {
        self.rolls[self.current_roll] = pins;
        self.current_roll += 1;
    }

    fn is_spare(&self, frame_index: usize) -> bool {
        self.rolls[frame_index] + self.rolls[frame_index + 1] == 10
    }
}

fn new_game() -> Game {
    Game {
        rolls: [0; 21],
        current_roll: 0,
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn gutter_game() {
        let mut g = new_game();
        roll_many(&mut g, 20, 0);
        assert_eq!(0, g.score());
    }

    #[test]
    fn all_one() {
        let mut g = new_game();
        roll_many(&mut g, 20, 1);
        assert_eq!(20, g.score());
    }

    #[test]
    fn one_spare() {
        let mut g = new_game();
        roll_spare(&mut g);
        g.roll(3);
        roll_many(&mut g, 17, 0);
        assert_eq!(16, g.score());
    }

    #[test]
    fn one_strike() {
        let mut g = new_game();
        roll_strike(&mut g);
        g.roll(3);
        g.roll(4);
        roll_many(&mut g, 16, 0);
        assert_eq!(24, g.score());
    }

    #[test]
    fn perfect_game() {
        let mut g = new_game();
        roll_many(&mut g, 12, 10);
        assert_eq!(300, g.score());
    }

    fn roll_strike(g: &mut Game) {
        g.roll(10);
    }

    fn roll_many(g: &mut Game, n: i32, pins: i32) {
        for _ in 0..n {
            g.roll(pins);
        }
    }

    fn roll_spare(g: &mut Game) {
        g.roll(5);
        g.roll(5);
    }
}
