
def is_obj_string_valid(string_check):
    """
    Validate whether a given string represents a simple dictionary-like structure.

    Validation Rules:
        1. The string must start with '{' and end with '}'.
        2. There must be exactly one opening '{' and one closing '}'.
        3. The number of colons (:) must equal the number of commas (,) + 1.

    Args:
        string_check (str): The string to check.

    Returns:
        int: 1 if the string is valid, 0 otherwise.
    """
    # Ensure string starts with '{' and ends with '}'
    if string_check[-1] != '}' or string_check[0] != '{':
        return 0

    # Ensure exactly one opening and one closing curly bracket
    if string_check.count("{") > 1 or string_check.count("}") > 1:
        return 0

    # Ensure the number of ":" matches number of "," + 1
    if string_check.count(":") != (string_check.count(",") + 1):
        return 0

    # Passed all checks → string is valid
    return 1

def is_obj_key_value_valid(key, vl):
    """
    Validate the key and value pair.

    Validation Rules:
        1. Keys must not be empty.
        2. Keys must not contain spaces.
        3. If the key is in ["tries", "word_length"], the value must be a digit greater than 1.

    Args:
        key (str): The key to validate.
        vl (str): The value to validate.

    Returns:
        int: 1 if the key-value pair is valid, 0 otherwise.
    """
    # Initialize keys that require numeric validation
    check_keys = ["tries", "word_length"]

    # Reject empty keys
    if not len(key):
        return 0

    # Reject keys containing spaces
    if " " in key:
        return 0

    # If key requires numeric validation, ensure value is a digit > 1
    if key in check_keys:
        if not vl.isdigit() or int(vl) <= 1:
            return 0

    # Passed all validation rules
    return 1

def string_to_obj(dict_string):
    """
    Convert a dictionary-like string into a Python dictionary.
    
    Args:
        dict_string (str): The string to convert.

    Returns:
        dict: A new dictionary of settings if valid.
              Prints "Invalid settings" and returns None if validation fails.
    """
    settings = {}

    # Remove curly braces and split into key-value pairs
    dict_string = dict_string.strip("{}").split(",")

    # Iterate over each key-value pair
    for pair in dict_string:
        # Split string into key and value
        key, vl = pair.split(":")

        # Remove surrounding whitespace
        key = key.strip()
        vl = vl.strip()

        # Validate key-value pair and check for duplicate keys
        if not is_obj_key_value_valid(key, vl) or key in settings:
            return print("Invalid settings")

        # Convert numeric values to int
        if vl.isdigit():
            vl = int(vl)

        # Store key-value pair in settings dictionary
        settings[key] = vl

    return settings
def change_settings(old_settings):
    """
    Update the game settings dictionary with new user input.

    Args:
        old_settings (dict): The current game settings.

    Returns:
        None: Prints status messages based on success or failure.
    """
    # Prompt the user to enter new settings
    new_settings = input("Enter settings:\n")

    # Validate that the input is in dictionary-like format
    if not is_obj_string_valid(new_settings):
        return print("Invalid settings")
    
    # Convert the input string into a dictionary
    result_dictionary = string_to_obj(new_settings)

    # If conversion was successful, update the settings
    if result_dictionary:
        for key, setting in result_dictionary.items():
            old_settings[key] = setting
        print("Settings were updated")

def print_game_open_message(tries, word_length):
    """
    Print the opening message of the game.

    Args:
        tries (int): The number of tries allowed in the game.
        word_length (int): The length of the word to guess.
    """
    # Print the welcome message and number of tries
    print(f"Welcome to Wordle! You have {tries} tries", end=' ')
    print("to guess the word.")
    
    # Print the word length information
    print(f"The word is {word_length} letters long.")

def update_feedback(feedback_obj, guess, is_first):
    """
    Update the feedback object based on the player's guess.

    Args:
        feedback_obj (dict): Dictionary containing 'word_to_guess' and 'feedback'.
        guess (str): The user's guessed word.
        is_first (bool): True if this is the first call, False otherwise.
    """
    word_to_guess = feedback_obj['word_to_guess']
    feedback = feedback_obj['feedback']
    length = len(guess)

    # Iterate over each letter of the guessed word
    for index in range(length):
        if is_first:
            # First call: check if letter is in the correct position
            if word_to_guess[index] == guess[index]:
                # Update feedback with correct letter
                feedback += guess[index]
                # Remove correctly guessed letter to prevent duplicates
                word_to_guess = " ".join([
                    word_to_guess[:index], word_to_guess[index + 1:]
                ])
            else:
                # Mark incorrect letter
                feedback += "-"
        else:
            # Subsequent call: check if letter exists elsewhere in the word
            if word_to_guess.find(guess[index]) != -1:
                # Only update if feedback currently has "-"
                if feedback[index] == '-':
                    feedback = "+".join([feedback[:index], feedback[index + 1:]])

    # Update the feedback object with modified feedback and remaining letters
    feedback_obj['feedback'] = feedback
    feedback_obj['word_to_guess'] = word_to_guess
def append_guess_feedback(word_to_guess, guess, game_object):
    """
    Process the user's guess and append the resulting feedback to the game object.

    Args:
        word_to_guess (str): The target word for the current game round.
        guess (str): The user's guessed word.
        game_object (dict): Dictionary storing game details, including 'feedbacks' list.
    """
    feedback = ""
    is_first = True

    # Check if the guess is exactly correct
    if guess == word_to_guess:
        # Append the correct guess directly to feedbacks
        game_object['feedbacks'].append(guess)
    else:
        # Initialize feedback object
        feedback_obj = {"word_to_guess": word_to_guess, "feedback": feedback}

        # Update feedback: first for correct positions, then for letters in wrong positions
        update_feedback(feedback_obj, guess, is_first)
        update_feedback(feedback_obj, guess, not is_first)

        # Append the computed feedback string to feedbacks
        game_object['feedbacks'].append(feedback_obj['feedback'])

    # Display the latest feedback to the user
    print(game_object['feedbacks'][-1])

def check_the_guess(game_object, word_to_guess, words, word_length):
    """
    Evaluate the player's guess and update the game state.

    Args:
        game_object (dict): Dictionary holding current game data, including 'guess' and 'player_tries'.
        word_to_guess (str): The target word for this game round.
        words (list): List of valid words allowed in the game.
        word_length (int): The required length of the guessed word.
    """
    # Extract the current guess
    guess = game_object['guess']

    # Correct guess: update status and feedback
    if guess == word_to_guess:
        game_object['status'] = 0
        game_object['player_tries'] += 1
        append_guess_feedback(word_to_guess, guess, game_object)
    
    # Invalid guess: wrong length or not in word list
    elif len(guess) != word_length or guess not in words:
        print("Invalid guess")
    
    # Valid but incorrect guess: increment tries and append feedback
    else:
        game_object['player_tries'] += 1
        append_guess_feedback(word_to_guess, guess, game_object)

def print_array(arr):
    """
    Print each element of a list on a separate line.

    Args:
        arr (list): The list of elements to print.
    """
    for st in arr:
        print(st)

def check_for_win(game_object, word_to_guess):
    """
    Determine if the game was won or lost and display the results.

    Args:
        game_object (dict): Dictionary containing current game details, including 'status' and 'feedbacks'.
        word_to_guess (str): The target word for this game round.

    Returns:
        int: Number of tries if the player won, 0 otherwise.
    """
    # Check if the game was won
    if game_object['status'] == 0:
        print("You win!\nGame over!")
        print_array(game_object['feedbacks'])
        return game_object['player_tries']
    
    # Game lost: print the correct word and all feedbacks
    else:
        print("You lost! The word was", word_to_guess)
        print("Game over!")
        print_array(game_object['feedbacks'])
    
    return 0

def play_game(settings, word_to_guess, words):
    """
    Run the main game loop, prompting the player for guesses and checking them.

    Args:
        settings (dict): Global settings, including 'tries' and 'word_length'.
        word_to_guess (str): The word the player must guess.
        words (list): List of valid words for the game.

    Returns:
        int: Number of tries used if the player wins, 0 otherwise.
    """
    # Extract the allowed number of tries and initialize the game object
    tries = settings['tries']
    game_object = {
        'status': 1,          # 1 = ongoing, 0 = won
        'player_tries': 0,    # Number of attempts made
        'guess': "",          # Current guess
        'feedbacks': []       # Feedback for each guess
    }

    # Print the opening game message
    print_game_open_message(settings['tries'], settings['word_length'])

    # Main game loop: runs until max tries reached or game is won
    while game_object["player_tries"] < tries and game_object['status']:
        # Prompt player for a guess
        game_object['guess'] = input("Guess a word:\n")

        # Validate and process the guess
        check_the_guess(game_object, word_to_guess, words, settings['word_length'])

    # Determine win/loss and return result
    return check_for_win(game_object, word_to_guess)

def validate_user_input(word_to_guess, words, word_length):
    """
    Check if the user's input is a valid word for the game.

    Args:
        word_to_guess (str): The player's input word.
        words (list): List of valid words for the game.
        word_length (int): The required length for a valid word.

    Returns:
        int: 1 if the word is valid, 0 otherwise.
    """
    # Check if the word exists in the allowed words list
    if word_to_guess not in words:
        print("That's not a word!")
        return 0

    # Check if the word has the correct length
    elif len(word_to_guess) != word_length:
        print("That word is in the wrong length!")
        return 0

    # Word is valid
    return 1

def store_player_details(players, settings, words):
    """
    Collect player details, validate input, and start the game.

    Args:
        players (dict): Dictionary storing all players and their statistics.
        settings (dict): Global game settings including 'tries' and 'word_length'.
        words (list): List of valid words for the game.
    """
    # Prompt for player's name
    name = input("Enter player's name:\n")

    # Prompt for the word to guess
    word_to_guess = input("Enter a word:\n")

    # Validate the entered word
    if validate_user_input(word_to_guess, words, settings['word_length']):
        # Create a new player if they do not already exist
        if name not in players:
            players[name] = {
                'games_played': 1,
                'wins': 0,
                'tries': 0,
            }
        # Increment games_played for existing players
        else:
            players[name]['games_played'] += 1

        # Start the game and retrieve the number of tries taken
        win_tries = play_game(settings, word_to_guess, words)

        # Update player's stats if they won
        if win_tries:
            players[name]['tries'] += win_tries
            players[name]['wins'] += 1

def sort_keys_alphabetic(to_sort):
    """
    Return a new dictionary sorted by the alphabetical order of its keys.

    Args:
        to_sort (dict): The dictionary to sort.

    Returns:
        dict: A new dictionary with keys in alphabetical order.
    """
    # Extract and sort the keys
    sorted_keys = sorted(to_sort.keys())

    # Build and return a new dictionary with sorted keys
    return {key: to_sort[key] for key in sorted_keys}

def sort_by_win_rate(players):
    """
    Return a list of players sorted by their win rate.

    Args:
        players (dict): Dictionary containing player statistics, including 'wins' and 'games_played'.

    Returns:
        list: A list of tuples (player_name, win_rate) sorted by win rate and name.
    """
    # Initialize a list to hold player names and win rates
    sorted_players = []

    # Calculate win rates and populate the list
    for player in players.keys():
        wins = players[player]['wins']
        games_played = players[player]['games_played']
        win_rate = round((wins / games_played) * 100, 2)
        sorted_players.append((player, win_rate))

    # Sort by win rate descending, then by name ascending
    return sorted(sorted_players, key=lambda col: (-col[1], col[0]))

def print_settings(settings):
    """
    Print the game settings in alphabetical order of the keys.

    The function works by:
        - Sorting the keys of the settings dictionary alphabetically.
        - Iterating through the sorted keys and printing each key-value pair.

    Args:
        settings (dict): The game settings dictionary.
    """
    # Sort the settings by keys
    sorted_settings = sort_keys_alphabetic(settings)

    # Print each key-value pair
    for key in sorted_settings.keys():
        print(f"{key}: {sorted_settings[key]}")

def print_win_table(players):
    """
    Print the players' scoreboard with games played, win rate, and average tries.

    Args:
        players (dict): Dictionary containing player statistics.
    """
    print("Scoreboard:")

    # Iterate through players sorted by win rate
    for (name, win_rate) in sort_by_win_rate(players):
        tries = players[name]['tries']
        wins = players[name]['wins']

        # Calculate average tries (0 if no wins)
        avg_tries = 0 if wins == 0 else round(tries / wins, 2)

        # Format basic statistics
        st = "{0}: {1} games, {2:.2f}% win rate,".format(
            name,
            players[name]['games_played'],
            win_rate,
        )

        # Add average tries information
        if wins == 0:
            st = " ".join([st, "NaN:-) average tries"])
        else:
            st = " ".join([st, f"{avg_tries:.2f} average tries"])

        # Print the formatted statistics
        print(st)

def handle_choice(choice, settings, players, words):
    """
    Execute the appropriate action based on the user's menu choice.

    Args:
        choice (int): The user's menu choice.
        settings (dict): The global game settings dictionary.
        players (dict): Dictionary containing all player statistics.
        words (list): List of valid words for the game.
    """
    if choice == 1:
        change_settings(settings)
    elif choice == 2:
        store_player_details(players, settings, words)
    elif choice == 3:
        print_settings(settings)
    elif choice == 4:
        print_win_table(players)
    else:
        # Invalid menu choice
        print("Invalid option")


def print_menu():
    """
    Display the main menu options to the user.

    The function works by:
        - Printing each available menu option with a corresponding number.
        - Options include: Exit, Update settings, Play, View settings, and Scoreboard.
    """
    print("Choose an option:")
    print("0. Exit")
    print("1. Update settings")
    print("2. Play")
    print("3. View settings")
    print("4. Scoreboard")

def main():
    """
    Start the Wordle game program and handle user interaction.
    """
    # Initialize game settings
    settings = {"tries": 6, "word_length": 5, "file_path": "words.txt"}

    # Initialize players data structure
    players = {}

    # Main loop for the menu
    while True:
        # Load the words list from file
        file = open(settings['file_path'], "r")
        words = file.read().splitlines()

        # Display the menu
        print_menu()

        # Get user input
        choice = input()

        # Skip invalid (non-digit) input
        if not choice.isdigit():
            continue

        choice = int(choice)

        # Exit if user chooses 0
        if choice == 0:
            file.close()
            break

        # Handle valid menu choice
        handle_choice(choice, settings, players, words)

   
if __name__ == "__main__":

    main()