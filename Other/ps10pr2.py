#
# ps10pr2.py (Problem Set 10, Problem 2)
#
# A Connect-Four Player class 
# Allison Bell: bbella@bu.edu

from ps10pr1 import Board

# write your class below

class Player:

    def __init__(self, checker):
        """ makes player class stores checker and # of moves"""

        assert(checker == 'X' or checker == 'O')

        self.checker = checker

        self.num_moves = 0

    def __repr__(self):
        """prints player class as Player 'checker'"""
        s = 'Player ' + self.checker

        return s

    def opponent_checker(self):
        """Tells the opponent checker based off self checker"""

        if self.checker == 'O':
            return 'X'
        if self.checker == 'X':
            return 'O'

    def next_move(self, board):
        """ takes a column as input from a player. returns column if
            possible play. Asks again if not.
        """

        while True:

            i = int(input('Enter a column: ' ))

            if board.can_add_to(i) == True:
                break

            print('Try again!')

        self.num_moves += 1

        return i
        
