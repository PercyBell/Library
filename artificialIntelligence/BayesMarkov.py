import random

def matrixPrint(m):

    l = [ x for x in m.keys()]

    for j in l:
        print('--',j,'--', end = '')
    print()

    for i in l:
        print(i, ': ', end = '')
        for j in l:
            print(round(m[i][j], 3),' | ', end = '')

        print()

            

def multiplyM(X, Y):
    l = [ x for x in X.keys()]
    result = {j:{k:0.0 for k in l} for j in l}
    
    # iterate through rows of X
    for i in l:
       # iterate through columns of Y
       for j in l:
           # iterate through rows of Y
           for k in l:
               result[i][j] += X[i][k] * Y[k][j]
    return result
    

class Action:

    def __init__ (self, name, alt, win, lose):

        self.name = name
        self.alternates = alt
        self.winLose = {'win':win, 'lose':lose}

    def validateAlt(self, alt):

        if alt in self.alternates:
            return self.name

        else:
            return None

    def winsAgainst(self, move):

        if move in self.winLose['win']:
            #print('l')
            return True

        if move in self.winLose['lose']:
            #print('w')
            return False

        else:
            #print('t')
            return None



class MarkovChain:

    def __init__(self, actions):
        ''' actions must be a list of Action class type'''
        # A is the opponent
        # B is self

        self.actions = actions
        self.actionsStr = [a.name for a in actions]

        #print(self.actionsStr)

        self.movesA = '' # oppontents moves
        self.movesAM = { y.name:[0,{z.name:0 for z in actions}] for y in actions}           
        self.movesB = '' # own moves
        self.movesABM = { y.name:[0,{z.name:0 for z in actions}] for y in actions}

        #self.matrixA = [ [0.0] * len(self.actions) for i in range(len(actions))]
        #self.matrixAB = [ [0.0] * len(self.actions) for i in range(len(actions))]
        self.matrixA = { y.name:{z.name:0.0 for z in actions} for y in actions}
        self.matrixAB = { y.name:{z.name:0.0 for z in actions} for y in actions}

    def factoryReset(self):

        self.movesA = '' # oppontents moves
        self.movesAM = { y.name:[0,{z.name:0 for z in actions}] for y in actions}           
        self.movesB = '' # own moves
        self.movesABM = { y.name:[0,{z.name:0 for z in actions}] for y in actions}

        self.matrixA = { y.name:{z.name:0.0 for z in actions} for y in actions}
        self.matrixAB = { y.name:{z.name:0.0 for z in actions} for y in actions}

    def setupA(self):

        if self.movesA == '':
            print("No moves to model")
            return None

        else:

            for i in range(len(self.movesA)-1):

                temp = self.movesA[i]
                nex = self.movesA[i+1]

                self.movesAM[temp][0] += 1
                self.movesAM[temp][1][nex] += 1

    def setupB(self):

        if self.movesA == '' or self.movesB == '':
            print("no moves to model")
            return None


        for i in range(len(self.movesA)-1):
            j = i + 1

            tempA = self.movesA[j] # their last move
            tempB = self.movesB[i] # my last move

            self.movesABM[tempB][0] += 1
            self.movesABM[tempB][1][tempA] += 1
            

        


    def updateMatrix(self, A = True):

        if A == True:
            mx = self.matrixA
            mo = self.movesAM

        else:
            mx = self.matrixAB
            mo = self.movesABM
            
        e = 0
        for i,v in mo.items():
            y = 0
            for j,l in v[1].items():

                if v[0] != 0:
                    mx[i][j] = l/v[0]

                else:
                    mx[i][j] = 0.0

                y += 1
            e += 1

            

    def extrapolate(self, moves):

        m = self.matrixA.copy()

        for i in range(moves-1):

            m = multiplyM(m , self.matrixA)

        return m


    def getProb(self, move, selfish = True):

        if move not in self.actionsStr:
            print("something has gone very wrong")
            return None

        if selfish == True:
            return self.matrixA[move]
        else:
            return self.matrixAB[move]

                    
    def updateMoves(self, moveA, moveB):
        self.movesA += moveA
        self.movesB += moveB

    def updateProbs(self, moveA, moveB): # NEVER CALL UPDATE MOVES BEFORE OR AFTER

        if self.movesA == '' or self.movesB == '':
            #print("Dont call this function on empty movesets")
            self.updateMoves(moveA, moveB)
            return None
        
        lastA = self.movesA[-1]
        lastB = self.movesB[-1]

        self.movesAM[lastA][0] += 1
        self.movesAM[lastA][1][moveA] += 1
        self.movesABM[lastB][0] += 1
        self.movesABM[lastB][1][moveA] += 1

        self.updateMatrix(A = True)
        self.updateMatrix(A = False)

        self.updateMoves(moveA, moveB)

    def resetAndRemake(self, movesetA, movesetB):

        self.movesA = movesetA
        self.movesB = movesetB

        self.setupA()
        self.setupB()

        self.updateMatrix(A = True)
        self.updateMatrix(A = False)

        

def miniRandomPlayer(times):

    x = ''

    for i in range(times):
        x += random.choice('RPS')
    
    return x



##R = Action('R', None, 'S', 'P')
##S = Action('S', None, 'P', 'R')
##P = Action('P', None, 'R', 'S')
##
##m = MarkovChain([R,S,P])
##
##m.movesA = 'PSPRR'
##m.movesB = 'RPPPR'
##
##m.setupB()
##m.setupA()
###print(m.movesABM)
##
##m.updateMatrix(A = True)
##m.updateMatrix(A = False)
##matrixPrint(m.matrixA)
##print()
##matrixPrint(m.matrixAB)
##
#####m.matrix = [[0.9, 0.075, 0.025],[0.15, 0.8, 0.05], [0.25,0.25,0.5]]
#####print(m.extrapolate(100))

        
