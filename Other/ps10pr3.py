#
# ps10pr3.py (Problem Set 10, Problem 3)
#
# Playing the Game  
#

from ps10pr1 import Board
from ps10pr2 import Player
import random
    
def connect_four(player1, player2):
    """ Plays a game of Connect Four between the two specified players,
        and returns the Board object as it looks at the end of the game.
        inputs: player1 and player2 are objects representing Connect Four
                  players (objects of the Player class or a subclass of Player).
                  One player should use 'X' checkers and the other should
                  use 'O' checkers.
    """
    # Make sure one player is 'X' and one player is 'O'.
    if player1.checker not in 'XO' or player2.checker not in 'XO' \
       or player1.checker == player2.checker:
        print('need one X player and one O player.')
        return None

    print('Welcome to Connect Four!')
    print()
    board = Board(6, 7)
    print(board)
    
    while True:
        if process_move(player1, board):
            return board

        if process_move(player2, board):
            return board


def process_move(player, board):
    """processes the player's move deciding on wins losses or ties resulting"""

    print(str(player) + "'s turn")
    space = player.next_move(board)

    board.add_checker(player.checker, space)
    print()
    print(board)



    if board.is_win_for(player.checker) == True:
        print(str(player) + ' wins in ' + str(player.num_moves) + ' moves')
        print('Congratulations!')
        return True

    else:

        if board.is_full() == True:
            print("It's a tie!")
            return True

        else:
            return False


class RandomPlayer(Player):
    """ Class for random player from the normal player class"""
    def __init__(self, checker):
        """Starts random player"""

        super().__init__(checker)
        self.num_moves = 0

    def next_move(self, board):
        """Gives random player's next move"""

        lc = [i for i in range(board.width) if board.can_add_to(i) == True]
        #print(lc)

        col = random.choice(lc)
        self.num_moves += 1

        return col
