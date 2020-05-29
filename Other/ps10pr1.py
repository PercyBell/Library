#
# ps10pr1.py (Problem Set 10, Problem 1)
#
# Allison Bell: bbella@bu.edu
#

class Board:
    """ A class that manipulates and creates the game board
    """

    def __init__(self, height, width):
        """ sets up board class with height width and slots"""
        self.height = height
        self.width = width

        self.slots = [[' ']*width for row in range(height)]

    def __repr__(self):
        """prints board with | inbetween tiles and numbered columns"""

        s = ''

        for row in range(self.height):
            s += '|'

            for col in range(self.width):

                s += self.slots[row][col] + '|'

            s += '\n'

        s += '-'*(2*self.width + 1) + '\n'

        for row in range(self.width):
            s += ' ' + str(row % 10)

        return s

    def add_checker(self, checker, col):
        """ adds checker to column. falls to lowest open spot"""

        assert(checker == 'X' or checker == 'O')
        assert(0 <= col < self.width)

        row = 0

        while self.slots[row][col] == ' ':
            row += 1

            if row == self.height:
                break
            
        if row > 0:
            self.slots[row - 1][col] = checker

    def reset(self):
        """makes the board all spaces again"""

        for row in range(self.height):
            for col in range(self.width):
                self.slots[row][col] = ' '

        #TEST

    def add_checkers(self, columns):
        """ Adds checkers to desired columns"""
        checker = 'X'   # start by playing 'X'

        for col_str in columns:
            col = int(col_str)

            if 0 <= col < self.width:
                self.add_checker(checker, col)

            if checker == 'X':
                checker = 'O'
            else:
                checker = 'X'

    def can_add_to(self, col):
        """checks if a checker can be added to column"""

        if col < 0:
            return False
        if (col + 1) > self.width:
            return False
        
        count = 0

        for row in range(self.height):
            if self.slots[row][col] == ' ':
                count += 1
        
        if count > 0:
            return True
        else:
            return False

    def is_full(self):
        """checks if board is full"""

        count = 0

        for row in range(self.height):
            for col in range(self.width):
                if self.slots[row][col] == ' ':
                    count += 1

        if count > 0:
            return False
        else:
            return True

    def remove_checker(self, col):
        """removes highest checker from column if there is one to remove"""

        row = 0

        while self.slots[row][col] == ' ':
            row += 1

            if row == self.height:
                row -= 1
                break
        
        self.slots[row][col] = ' ' 

    def is_win_for(self, checker):
        """ checks if it is a win for checker"""
        assert(checker == 'X' or checker == 'O')
        
        if self.is_horizontal_win(checker) == True:
            return True
        if self.is_vertical_win(checker) == True:
            return True
        if self.is_diagonal_win(checker) == True:
            return True
        if self.is_up_diagonal_win(checker) == True:
            return True
                                
        return False

    def is_horizontal_win(self, checker):
        """checks for horizontal win for checker"""
        
        for row in range(self.height):
            for col in range(self.width - 3):
                if self.slots[row][col] == checker and \
                   self.slots[row][col + 1] == checker and \
                   self.slots[row][col + 2] == checker and \
                   self.slots[row][col + 3] == checker:
                    return True

        return False

    def is_vertical_win(self, checker):
        """checks for verticle win for checker"""
        
        for row in range(self.height - 3):
            for col in range(self.width):
                if self.slots[row][col] == checker and \
                   self.slots[row + 1][col] == checker and \
                   self.slots[row + 2][col] == checker and \
                   self.slots[row + 3][col] == checker:
                    return True

        return False

    def is_diagonal_win(self, checker):
        """checks for diagonal win for checker"""
        
        for row in range(self.height - 3):
            for col in range(self.width - 3):
                if self.slots[row][col] == checker and \
                   self.slots[row + 1][col + 1] == checker and \
                   self.slots[row + 2][col + 2] == checker and \
                   self.slots[row + 3][col + 3] == checker:
                    return True

        return False
        
    def is_up_diagonal_win(self, checker):
        """checks for up diagonal win for checker"""
        
        for row in range(self.height - 3):
            for col in range(2, self.width):
                if self.slots[row][col] == checker and \
                   self.slots[row + 1][col - 1] == checker and \
                   self.slots[row + 2][col - 2] == checker and \
                   self.slots[row + 3][col - 3] == checker:
                    return True

        return False
