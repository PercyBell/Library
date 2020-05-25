# Player class

from BayesMarkov import *

class Player:

    def __init__(self, possibleActions):

        self.name = "Human Player"
        self.possibleActions = [a.name for a in possibleActions]
        self.pastActions = ''
        self.numActions = 0
        self.numWins = 0
        self.adversaryActions = ''

        self.actionList = possibleActions

    def findAction(self, act):

        for i in self.actionList:
            if i.name == act:
                return i

        print("Action Does not Exist :/")

        return None

    def nextMove(self):

        while True:
            move =  str(input('Enter an action: ')).upper()

            if move in self.possibleActions:
                break

            print ("Not a possible action")

        #self.pastActions += move
        self.numActions += 1

        return move

    def updateMoves(self, moveA, moveB):

        self.pastActions += moveA
        self.adversaryActions += moveB

    def simulate(self, movesetA, movesetB):

        self.pastActions = movesetA
        self.adversaryActions = movesetB

    def endGame(self):

        print(self.name)
        #print(self.pastActions)
        
    def __repr__(self):

        s = self.name + '\n'
        #s += self.pastActions + '\n'

        return s

        
class PlayerAI(Player):

    def __init__(self, possibleActions):

        Player.__init__(self, possibleActions)

    def nextMove(self):

        return None

class MarkovPlayer(PlayerAI):

    def __init__(self, possibleActions):

        PlayerAI.__init__(self, possibleActions)
        self.name = "Markov Player"
        
        self.Markov = MarkovChain(possibleActions)

    def nextMove(self, selfish = True):

        if self.pastActions == '':
            return random.choice(self.possibleActions)

        if selfish == True:
            lastMove = self.adversaryActions[-1]
            
        else:
            lastMove = self.pastActions[-1]

        probRow = self.Markov.getProb(lastMove, selfish)

        mostLikely = max([[y,x] for x,y in probRow.items()])[1]

        x = random.choice(self.findAction(mostLikely).winLose['lose'])

        return x

    def updateMoves(self, moveA, moveB):

        self.pastActions += moveA
        self.adversaryActions += moveB

        self.Markov.updateProbs(moveB, moveA)

    def simulate(self, movesetA, movesetB):

        self.pastActions = movesetA
        self.adversaryActions = movesetB

        self.Markov.resetAndRemake(movesetA, movesetB)

    def endGame(self):

        print(self.name)
        #print(self.pastActions)
        print('Probabilities on self')
        matrixPrint(self.Markov.matrixA)
        print('Probabilities on opp ')
        matrixPrint(self.Markov.matrixAB)

        print(self.Markov.actionsStr)

class RandomPlayer(PlayerAI):

    def __init__(self, possibleActions):

        PlayerAI.__init__(self, possibleActions)
        self.name = 'Random Player'


    def nextMove(self):

        return random.choice(self.possibleActions)

class SamePlayer(PlayerAI):

    def __init__(self, possibleActions):

        PlayerAI.__init__(self, possibleActions)
        self.name = 'Same Move Always Player'

        self.choice = random.choice(self.possibleActions)


    def nextMove(self):

        return self.choice

class ProbabalisticPlayer(PlayerAI):
    
    def __init__(self, possibleActions):

        PlayerAI.__init__(self, possibleActions)
        self.name = 'Probabalistic Player'

    def nextMove(self):

        acts = ['R', 'S', 'S', 'S', 'S', 'P', 'P', 'P', 'P', 'P']

        return random.choice(acts)

class WaitingMarkovPlayer(MarkovPlayer):

    def __init__(self, possibleActions):

        MarkovPlayer.__init__(self, possibleActions)
        self.name = 'Markov Player Waiting'

    def nextMove(self, selfish = True):

        if len(self.pastActions) <= 100:
            return random.choice(self.possibleActions)

        if selfish == True:
            lastMove = self.adversaryActions[-1]
            
        else:
            lastMove = self.pastActions[-1]

        probRow = self.Markov.getProb(lastMove, selfish)

        mostLikely = max([[y,x] for x,y in probRow.items()])[1]

        x = random.choice(self.findAction(mostLikely).winLose['lose'])

        return x

class RandomMarkov(MarkovPlayer):

    def __init__(self, possibleActions):

        MarkovPlayer.__init__(self, possibleActions)
        self.name = 'Markov Player Random'


    def nextMove(self, selfish = True):

        if len(self.pastActions) == 0:
            return random.choice(self.possibleActions)

        if selfish == True:
            lastMove = self.adversaryActions[-1]
            
        else:
            lastMove = self.pastActions[-1]

        probRow = self.Markov.getProb(lastMove, selfish)

##        acts = [x for x,y in probRow.items()]
##        probs = [y for x,y in probRow.items()]
##
##        if sum(probs) == 0:
##            return random.choice(self.possibleActions)
##
##        choices = [[a] * int(p * 100) for a, p in probRow.items()]
##        c = []
##        for i in choices:
##            c += i
##
##        choice = random.choice(c)
##
##        x = self.findAction(choice).winLose['lose']

        mostLikely = max([[y,x] for x,y in probRow.items()])[1]
        p = random.choice([True,True,True,True,False,True,True,True,True,False])

        if p != False:
            x = random.choice(self.findAction(mostLikely).winLose['lose'])
        else:
            x = mostLikely

        return x

class ReactionMarkov(MarkovPlayer):

    def __init__(self, possibleActions):

        MarkovPlayer.__init__(self, possibleActions)
        self.name = 'Markov Player Reaction'

    def nextMove(self, selfish = False):

        if len(self.pastActions) <= 100:
            return random.choice(self.possibleActions)

        if selfish == True:
            lastMove = self.adversaryActions[-1]
            
        else:
            lastMove = self.pastActions[-1]

        probRow = self.Markov.getProb(lastMove, selfish)

        mostLikely = max([[y,x] for x,y in probRow.items()])[1]

        x = random.choice(self.findAction(mostLikely).winLose['lose'])

        return x
    
class ExpectiMarkovPlayer(MarkovPlayer):
    
    def __init__(self, possibleActions):

        MarkovPlayer.__init__(self, possibleActions)
        self.name = 'ExpectiMax Marcov'

    def score(self, moveA, moveB):
        '''Does my move win against my opponents?'''

        a = self.findAction(moveA)
        #b = self.findAction(moveB)

        wlt = a.winsAgainst(moveB)

        if wlt == True:
            return 100

        if wlt == False:
            return -100

        else:
            return 0

    def probM(self, a, b, M):

        return M[a][b]
        

    def bestMove(self):

        a = self.pastActions[-1]
        b = self.adversaryActions[-1]
        matrix = self.Markov.matrixA.copy()

        def expectimax(movesA, movesB, matrix, depth, player, first = False):
            if depth == 0:
                if len(movesA) != len(movesB):
                    print("Everything has gone very wrong")
                    return None
                return self.score(movesA[-1], movesB[-1]) # if end is reached score last moves

            lastA = movesA[-1]
            lastB = movesB[-1]
            
            if player == 'Max':

                actions = self.possibleActions
                bestAct = None
                bestScore = -(float("inf"))

                for actA in actions:
                    a = movesA + actA

                    score = expectimax(a, movesB, matrix, depth-1, 'Exp')

                    if score >= bestScore:
                        bestScore = score
                        bestAct = actA

                if first == True:
                    return bestAct
                else:
                    return bestScore

            if player == 'Exp':

                actions = self.possibleActions
                bestScore = float("inf")
                tot = 0

                for actB in actions:

                    b = movesB + actB

                    score = expectimax(movesA, b, matrix, depth-1, 'Max')

                    tot += self.probM(actB, lastB, matrix) * score

                    #print(self.probM(actB, lastB, matrix) * score)

                return tot

        return expectimax(a, b, matrix, 2, 'Max', first = True)
            

    def nextMove(self, selfish = False):

        if len(self.pastActions) <= 100:
            return random.choice(self.possibleActions)

        x = self.bestMove()

        #rotate?
        #x = self.findAction(x).winLose['win']

        return x

class ValueItterMarkov(MarkovPlayer):
    pass
