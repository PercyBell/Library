#
# ps10pr4.py (Problem Set 10, Problem 4)
#
# AI Player for use in Connect Four
#

import random  
from ps10pr3 import *

class AIPlayer(Player):
    def __init__(self, checker, tiebreak, lookahead):
        """ Starts AI player with checker, tiebreaker and lookahead
        """
        assert(checker == 'X' or checker == 'O')
        assert(tiebreak == 'LEFT' or tiebreak == 'RIGHT' or tiebreak == 'RANDOM')
        assert(lookahead >= 0)

        super().__init__(checker)

        self.tiebreak = tiebreak
        self.lookahead = lookahead

    def __repr__(self):
        """Returns string representation of player"""
        return 'Player ' + str(self.checker) + ' (' + str(self.tiebreak) + ', ' + str(self.lookahead) +')'

    def max_score_column(self, scores):
        """Gives max column score"""

        s = []

        for i in range(len(scores)):
            if scores[i] == max(scores):
                s += [i]

        if self.tiebreak == 'RANDOM':
            col = random.choice(s)
        if self.tiebreak == 'LEFT':
            col = s[0]
        if self.tiebreak == 'RIGHT':
            col = s[-1]

        return col

    def scores_for(self, board):
        """Gives column scores based on its lookahead"""

        scores = [50] * board.width

        for col in range(board.width):
            if board.can_add_to(col) == False:
                scores[col] = -1
            elif board.is_win_for(self.checker) == True:
                scores[col] = 100
            elif board.is_win_for(self.opponent_checker()) == True:
                scores[col] = 0
            elif self.lookahead == 0:
                scores[col] = scores[col]

            else:
                board.add_checker(self.checker, col)
                opp = AIPlayer(self.opponent_checker(), self.tiebreak, self.lookahead - 1)
                opp_scores = opp.scores_for(board)
                scores[col] = abs(max(opp_scores) - 100)
                board.remove_checker(col)

        return scores

    def next_move(self, board):
        """Finds AI's best possible move outputs column"""

        scores = self.scores_for(board)

        col = self.max_score_column(scores)

        self.num_moves += 1

        return col
                

