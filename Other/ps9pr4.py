#
# Problem 4 Problem Set 9
#
# Allison Bell: bbella@bu.edu
#

import random

def create_dictionary(filename):
    """ creates a markov model dictionary
    """

    file = open(filename, 'r')

    words = []

    for line in file:
        line = line[:-1]
        line = line.split()

        words += line

    #print(words)

    file.close()

    d = {}
    current_word = '$'

    for next_word in words:
        if current_word not in d:
            d[current_word] = [next_word]
        else:
            d[current_word] += [next_word]

        if next_word[-1] in '.!?':
            current_word = '$'
        else:
            current_word = next_word

    return d

def generate_text(word_dict, num_words):
    """ uses the markov model to generate sentences
    """

    current_word = '$'

    for i in range(num_words):
        wordlist = word_dict[current_word]

        next_word = random.choice(wordlist)

        print(next_word, end = ' ')

        if next_word[-1] in '.!?':
            current_word = '$'
        else:
            current_word = next_word

    print()
