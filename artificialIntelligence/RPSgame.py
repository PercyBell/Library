from BayesMarkov import *
from PlayerRPS import *

def menu1():
    print("Which game would you like to play?")
    print("(0) Normal RPS")
    print("(1) RPS with Lizard Spock")
    print("(2) Extreme RPS")

def menu2(player):
    print("Who should player", player, "be?")
    print("(0) Human")
    print("(1) Random")
    print("(2) Same Move")
    print("(3) Probabalistic")
    print("(4) Simple Markov")
    print("(5) Reaction Markov")
    print("(6) Waiting Markov")
    print("(7) Random Markov")
    print("(8) Markov with Expectimax")

class RPSgame:

    def __init__(self, PlayerA, PlayerB, actions):

        self.actions = actions

        self.A = PlayerA
        self.B = PlayerB

        self.movesA = ''
        self.movesB = ''

        self.points = {'A':0, 'Tie':0, 'B':0}

    def find(self, move):

        for i in self.actions:
            if i.name == move:
                return i

        else:
            return None


    def turn(self, s = True):

        a = self.A.nextMove()
        b = self.B.nextMove()

        if s == True:
            print('A: ',a ,'B: ', b) 

        self.movesA += a
        self.movesB += b

        x = self.find(a)

        win = x.winsAgainst(b)

        if win == True:
            if s == True:
                print('Win for A')
            self.points['A'] += 1

        if win == False:
            if s == True:
                print('Win for B')
            self.points['B'] += 1

        if win == None:
            if s == True:
                print('Tie')
            self.points['Tie'] += 1

        if s == True:
            print(self.points)

        self.A.updateMoves(a,b)
        self.B.updateMoves(b,a)

    def startGame(self):

        turns = int(input('How many turns?: '))

        for i in range(turns):

            self.turn()

        print("-----Game Over-----")
        print(self.points)
        print()
        self.A.endGame()
        self.B.endGame()

    def startGameScilent(self, n):

        for i in range(n):
            self.turn(s = False)

        print("Game Over")
        print(self.points)
        print()
        self.A.endGame()
        self.B.endGame()

        
##
##
##
##R = Action('R', None, 'S', 'P')
##P = Action('P', None, 'R', 'S')
##S = Action('S', None, 'P', 'R')
##
##x = [R, P, S]
##
##A = Player(x)
##B = Player(x)
##
##game = RPSgame(A,B,x)
##
##def playRPS():
##
##    print("A normal game of rock, paper, scissors...")
##
##    R = Action('R', ['ROCK','R'], 'S', 'P')
##    S = Action('S', ['PAPER','P'], 'P', 'R')
##    P = Action('P', ['SCISSORS','S'], 'R', 'S')
##
##    x = [R, P, S]
##
##    A = Player(x)
##    B = Player(x)
##
##    game = RPSgame(A,B,x)
##    game.startGame()
##
##def playRPSLX(n, ex = False):
##
##    print("A game of rock, paper, scissors, lizard, spock...")
##
##    if ex == True:
##        print("Possible moves are: Rock(R), Paper(P), Scissors(Sc), and Spock(Sp)")
##
##    R = Action('R', ['ROCK','R'], ['S','L'], ['P','X'])
##    P = Action('P', ['PAPER','P'], ['R','X'], ['S','L'])
##    S = Action('S', ['SCISSORS','Sc'], ['P','X'], ['R','X'])
##    L = Action('L', ['LIZARD','L'], ['X','P'], ['R','S'])
##    X = Action('X', ['SPOCK','SP'], ['R','S'], ['P','L'])
##
##    x = [R,P,S,L,X]
##
##    A = MarkovPlayer(x)
##    B = RandomPlayer(x)
##
##    game = RPSgame(A,B,x)
##    game.startGameScilent(n)
##
##def playExtremeRPS(ex = False):
##
##    print("A game of extreme RPS...")
##
##    if ex == True:
##        print("Possible moves are: Rock(R), Fire(F),")
##
##    Rock = Action('R', ['ROCK','R'], ['F','S','N','H','T','W','G'], ['P','A','E','D','I','L','G'])
##    Fire = Action('F', ['FIRE','F'], ['S','N','H','T','W','O','P'], ['A','E','D','I','L','G','R'])
##    Scissors = Action('S', ['SCISSORS','S'], ['N','H','T','W','O','P','A'], ['E','D','I','L','G','R','F'])
##    Snake = Action('N', ['SNAKE','SN','N'], ['H','T','W','O','P','A','E'], ['D','I','L','G','R','F','S'])
##    Human = Action('H', ['HUMAN','H'], ['T','W','O','P','A','E','D'], ['I','L','G','R','F','S','N'])
##    Tree = Action('T', ['TREE','T'], ['W','O','P','A','E','D','I'], ['L','G','R','F','S','N','H'])
##    Wolf = Action('W', ['WOLF','W'], ['O','P','A','E','D','I','L'], ['G','R','F','S','N','H','T'])
##    Sponge = Action('O', ['SPONGE','SPO','O'], ['P','A','E','D','I','L','G'], ['R','F','S','N','H','T','W'])
##    Paper = Action('P', ['PAPER','P'], ['A','E','D','I','L','G','R'], ['F','S','N','H','T','W','O'])
##    Air = Action('A', ['AIR','A'], ['E','D','I','L','G','R','F'], ['S','N','H','T','W','O','P'])
##    Water = Action('E', ['WATER','WA','E'], ['D','I','L','G','R','F','S'], ['N','H','T','W','O','P','A'])
##    Dragon = Action('D', ['DRAGON','D'], ['I','L','G','R','F','S','N'], ['H','T','W','O','P','A','E'])
##    Devil = Action('I', ['DEVIL','DE','I'], ['L','G','R','F','S','N','H'], ['T','W','O','P','A','E','D'])
##    Lightning = Action('L', ['LIGHTNING','L'], ['G','R','F','S','N','H','T'], ['W','O','P','A','E','D','I'])
##    Gun = Action('G', ['GUN','G'], ['R','F','S','N','H','T','W'], ['O','P','A','E','D','I','L'])
##
##    #['R','F','S','N','H','T','W','O','P','A','E','D','I','L','G']
##
##    x = [Rock, Fire, Scissors, Snake, Human, Tree, Wolf, Sponge, Paper, Air, Water, Dragon, Devil, Lightning, Gun]
##
##    A = Player(x)
##    B = Player(x)
##    
##    game = RPSgame(A,B,x)
##    game.startGame()
    
    
##def chooseGame():
##
##    menu1()
##
##    choice1 = int(input("Choice: "))
##
##    menu2('A')
##
##    choice2 = int(input("Choice: "))
##
##    menu2('B')
##
##    choice3 = int(input("Choice: "))
##
##    playerA = None
##    playerB = None
##
##    if choice1 == 0:
##        playRPS()
##
##    if choice1 == 1:
##        playRPSLX()
##
##    if choice1 == 2:
##        playExtremeRPS()
##
####R = Action('R', ['ROCK','R'], 'S', 'P')
####S = Action('S', ['PAPER','P'], 'P', 'R')
####P = Action('P', ['SCISSORS','S'], 'R', 'S')
####
####x = [R, P, S]

##R = Action('R', ['ROCK','R'], ['S','L'], ['P','X'])
##P = Action('P', ['PAPER','P'], ['R','X'], ['S','L'])
##S = Action('S', ['SCISSORS','Sc'], ['P','X'], ['R','X'])
##L = Action('L', ['LIZARD','L'], ['X','P'], ['R','S'])
##X = Action('X', ['SPOCK','SP'], ['R','S'], ['P','L'])
##
##x = [R,P,S,L,X]
##
##A = WaitingMarkovPlayer(x)
##B = ExpectiMarkovPlayer(x)
##
##game = RPSgame(A,B,x)
###game.startGame()
##game.startGameScilent(100000)
####
####
#####playRPSLX(1000)

def Play():

    menu1()

    choice1 = int(input("Choice: "))

    menu2('A')

    choice2 = int(input("Choice: "))

    menu2('B')

    choice3 = int(input("Choice: "))

    A = None
    B = None

    if choice1 == 0:
        R = Action('R', ['ROCK','R'], 'S', 'P')
        S = Action('S', ['PAPER','P'], 'P', 'R')
        P = Action('P', ['SCISSORS','S'], 'R', 'S')

        x = [R, P, S]

    if choice1 == 1:
        R = Action('R', ['ROCK','R'], ['S','L'], ['P','X'])
        P = Action('P', ['PAPER','P'], ['R','X'], ['S','L'])
        S = Action('S', ['SCISSORS','Sc'], ['P','X'], ['R','X'])
        L = Action('L', ['LIZARD','L'], ['X','P'], ['R','S'])
        X = Action('X', ['SPOCK','SP'], ['R','S'], ['P','L'])

        x = [R,P,S,L,X]

    if choice1 == 2:
        Rock = Action('R', ['ROCK','R'], ['F','S','N','H','T','W','G'], ['P','A','E','D','I','L','G'])
        Fire = Action('F', ['FIRE','F'], ['S','N','H','T','W','O','P'], ['A','E','D','I','L','G','R'])
        Scissors = Action('S', ['SCISSORS','S'], ['N','H','T','W','O','P','A'], ['E','D','I','L','G','R','F'])
        Snake = Action('N', ['SNAKE','SN','N'], ['H','T','W','O','P','A','E'], ['D','I','L','G','R','F','S'])
        Human = Action('H', ['HUMAN','H'], ['T','W','O','P','A','E','D'], ['I','L','G','R','F','S','N'])
        Tree = Action('T', ['TREE','T'], ['W','O','P','A','E','D','I'], ['L','G','R','F','S','N','H'])
        Wolf = Action('W', ['WOLF','W'], ['O','P','A','E','D','I','L'], ['G','R','F','S','N','H','T'])
        Sponge = Action('O', ['SPONGE','SPO','O'], ['P','A','E','D','I','L','G'], ['R','F','S','N','H','T','W'])
        Paper = Action('P', ['PAPER','P'], ['A','E','D','I','L','G','R'], ['F','S','N','H','T','W','O'])
        Air = Action('A', ['AIR','A'], ['E','D','I','L','G','R','F'], ['S','N','H','T','W','O','P'])
        Water = Action('E', ['WATER','WA','E'], ['D','I','L','G','R','F','S'], ['N','H','T','W','O','P','A'])
        Dragon = Action('D', ['DRAGON','D'], ['I','L','G','R','F','S','N'], ['H','T','W','O','P','A','E'])
        Devil = Action('I', ['DEVIL','DE','I'], ['L','G','R','F','S','N','H'], ['T','W','O','P','A','E','D'])
        Lightning = Action('L', ['LIGHTNING','L'], ['G','R','F','S','N','H','T'], ['W','O','P','A','E','D','I'])
        Gun = Action('G', ['GUN','G'], ['R','F','S','N','H','T','W'], ['O','P','A','E','D','I','L'])

        #['R','F','S','N','H','T','W','O','P','A','E','D','I','L','G']

        x = [Rock, Fire, Scissors, Snake, Human, Tree, Wolf, Sponge, Paper, Air, Water, Dragon, Devil, Lightning, Gun]

    
    if choice2 == 0:
        A = Player(x)
    if choice2 == 1:
        A = RandomPlayer(x)
    if choice2 == 2:
        A = SamePlayer(x)
    if choice2 == 3:
        A = ProbabalisticPlayer(x)
    if choice2 == 4:
        A = MarkovPlayer(x)
    if choice2 == 5:
        A = ReactionMarkov(x)
    if choice2 == 6:
        A = WaitingMarkovPlayer(x)
    if choice2 == 7:
        A = RandomMarkov(x)
    if choice2 == 8:
        A = ExpectiMarkovPlayer(x)

    if choice3 == 0:
        B = Player(x)
    if choice3 == 1:
        B = RandomPlayer(x)
    if choice3 == 2:
        B = SamePlayer(x)
    if choice3 == 3:
        B = ProbabalisticPlayer(x)
    if choice3 == 4:
        B = MarkovPlayer(x)
    if choice3 == 5:
        B = ReactionMarkov(x)
    if choice3 == 6:
        B = WaitingMarkovPlayer(x)
    if choice3 == 7:
        B = RandomMarkov(x)
    if choice3 == 8:
        B = ExpectiMarkovPlayer(x)

    game = RPSgame(A,B,x)
    game.startGame()
