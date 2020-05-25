from RPSgame import *

R = Action('R', ['ROCK','R'], 'S', 'P')
S = Action('S', ['PAPER','P'], 'P', 'R')
P = Action('P', ['SCISSORS','S'], 'R', 'S')

x = [R, P, S]

print("(RPS) Normal Markov(B) vs Reaction Markov(A) (10000 turns)")

B = MarkovPlayer(x)
A = ReactionMarkov(x)

game = RPSgame(A,B,x)
game.startGameScilent(1000)


print("(RPS) Normal Markov(A) vs Waiting Markov(B) (10000 turns)")

A = MarkovPlayer(x)
B = WaitingMarkovPlayer(x)

game = RPSgame(A,B,x)
game.startGameScilent(1000)

R = Action('R', ['ROCK','R'], ['S','L'], ['P','X'])
P = Action('P', ['PAPER','P'], ['R','X'], ['S','L'])
S = Action('S', ['SCISSORS','Sc'], ['P','X'], ['R','X'])
L = Action('L', ['LIZARD','L'], ['X','P'], ['R','S'])
X = Action('X', ['SPOCK','SP'], ['R','S'], ['P','L'])

x = [R,P,S,L,X]

print("(RPSLX) Normal Markov(A) vs ExpectiMax Markov(B) (10000 turns)")

A = MarkovPlayer(x)
B = ExpectiMarkovPlayer(x)

game = RPSgame(A,B,x)
game.startGameScilent(10000)






