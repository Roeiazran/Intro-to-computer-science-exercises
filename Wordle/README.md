# Wordle-Style Python Game

A simple Wordle-inspired game implemented in Python, allowing multiple players, customizable settings, and a scoreboard.

## Features

- Play Wordle with any custom word of specified length.
- Track multiple players with games played, wins, and average tries.
- Update game settings (number of tries and word length).
- View the current settings.
- Display a scoreboard sorted by win rate.
- **Guideline/Clue Feature**: Receive hints about letters in the correct or incorrect positions to help with guesses.

## Requirements

- Python 3.x
- A text file containing the list of valid words (default: `words.txt`).

## Setup

1. Clone the repository or download the code files.
2. Ensure `words.txt` exists in the project folder, containing one word per line.
3. Run the game:

```bash
python main.py
```

## Here's a running example:

```
Choose an option:
0. Exit
1. Update settings
2. Play
3. View settings
4. Scoreboard
2
Enter player's name:
yaniv
Enter a word:
blued
Welcome to Wordle! You have 6 tries to guess the word.
The word is 5 letters long.
Guess a word:
bulgy
b++--
Guess a word:
holed
--+ed
Guess a word:
blued
blued
You win!
Game over!
b++--
--+ed
blued
Choose an option:
0. Exit
1. Update settings
2. Play
3. View settings
4. Scoreboard
2
Enter player's name:
moshe
Enter a word:
sited
Welcome to Wordle! You have 6 tries to guess the word.
The word is 5 letters long.
Guess a word:
writs
--+++
Guess a word:
metre
-+t-+
Guess a word:
shies
s-+e-
Guess a word:
holed
---ed
Guess a word:
sited
sited
You win!
Game over!
--+++
-+t-+
s-+e-
---ed
sited
Choose an option:
0. Exit
1. Update settings
2. Play
3. View settings
4. Scoreboard
2
Enter player's name:
yaniv
Enter a word:
gigas
Welcome to Wordle! You have 6 tries to guess the word.
The word is 5 letters long.
Guess a word:
sines
-i--s
Guess a word:
sigma
+ig-+
Guess a word:
gigas
gigas
You win!
Game over!
-i--s
+ig-+
gigas
Choose an option:
0. Exit
1. Update settings
2. Play
3. View settings
4. Scoreboard
4
Scoreboard:
moshe: 1 games, 100.00% win rate, 5.00 average tries
yaniv: 2 games, 100.00% win rate, 3.00 average tries
Choose an option:
0. Exit
1. Update settings
2. Play
3. View settings
4. Scoreboard
1
Enter settings:
{ word_length : 4, tries : 10}
Settings were updated
Choose an option:
0. Exit
1. Update settings
2. Play
3. View settings
4. Scoreboard
3
file_path: words.txt
tries: 10
word_length: 4
Choose an option:
0. Exit
1. Update settings
2. Play
3. View settings
4. Scoreboard
0

```
