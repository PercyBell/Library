#
# Final Project: finalproject.py
#
# Allison Bell: bbella@bu.edu
#

from math import *

def clean_text(txt):
    """Cleans punctuation out of a string"""
    
    txt = txt.lower()
    txt = txt.replace('.','')
    txt = txt.replace(',','')
    txt = txt.replace('?','')
    txt = txt.replace('!','')
    txt = txt.replace('"','')
    txt = txt.replace("'",'')

    return txt

def stem(s):
    """ attempts to stem words (only works with common words. and sometimes not even then)"""
    if len(s) < 4:
        return s
    elif s[-2:] == 'er':
        if s[-3] == s[-4]:
            return s[:-3]
        else:
            return s[:-2]
    elif s[-3:] == 'ing':
        if len(s) < 5:
            return s[:-3]
        else:
            if s[-5] == s[-4]:
                return s[:-4]
            else:
                return s[:-3]
    elif s[-2:] == 'ly':
        return s[-3:]
    elif s[-2:] =='ed':
        if s == 'red':
            return s
        else:
            return s[:-3]
    elif s[-1] == 'y':
        if s[-2] == s[-3]:
            return s[:-2]
        else:
            return s[:-1] + 'i'
    elif s[-3:] == 'ies':
        return s[:-2]
    elif s[-1] == 's':
        return s[:-1]
    elif s[:2] == 'un':
        if s == 'under':
            return s
        else:
            return s[2:]
    
    else:
        return s


def compare_dictionaries(d1, d2):
    """compares to dictionaries giving a similarity log score"""
    score = 0
    total = 0

    for x in d1:
        total += d1[x]

    for x in d2:
        if x in d1:
            score += log(d1[x]/total)*d2[x]
        else:
            score += log(0.5/total)*d2[x]

    return score

def run_tests():
    """tests classify func"""
    source1 = TextModel('Dune')
    source1.add_file('Dune A.txt')
    source1.add_file('Dune B.txt')

    source2 = TextModel('Hitchhikers')
    source2.add_file('HG2GA.txt')
    source2.add_file('HG2GB.txt')

    new1 = TextModel('mine')
    new1.add_file('MeA.txt')
    new1.classify(source1, source2)

    new2 = TextModel('Dune2')
    new2.add_file('mystery B.txt')
    new2.classify(source1, source2)

    new3 = TextModel('Hitchhikers2')
    new3.add_file('mystery A.txt')
    new3.classify(source1, source2)

    new4 = TextModel('Dirk Gently')
    new4.add_file('dgh.txt')
    new4.classify(source1, source2)

    new5 = TextModel('Time Machine')
    new5.add_file('Wells.txt')
    new5.classify(source1, source2)
    # Add code for three other new models below.


class TextModel:

    def __init__(self, model_name):
        """Starts model with empty dictionaries"""

        self.name = model_name
        self.words = {}
        self.word_lengths = {}
        self.stems = {}
        self.sentence_lengths = {}
        self.punctuation = {}

    def __repr__(self):
        """gives the display for model"""
        s = 'text model name: ' + self.name + '\n'
        s += '  number of words: ' + str(len(self.words)) + '\n'
        s += '  number of word lengths: ' + str(len(self.word_lengths)) + '\n'
        s += '  number of stems: ' + str(len(self.stems)) + '\n'
        s += '  number of sentence lengths:' + str(len(self.sentence_lengths)) + '\n'
        s += '  number of punctuation: ' + str(len(self.punctuation))
        return s

    def add_string(self, s):
        """adds string to model"""

        for l in s:
            if l in '.,"?!:;':
                if l in self.punctuation:
                    self.punctuation[l] += 1
                else:
                    self.punctuation[l] = 1
        
        word_list = s.split(' ')
        count = 0
        for w in word_list:
            count += 1
            if len(w) > 0:
                if w[-1] in '.?!':
                    if count in self.sentence_lengths:
                        self.sentence_lengths[count] += 1
                    else:
                        self.sentence_lengths[count] = 1
                    count = 0

                
        
        word_list = clean_text(s)
        word_list = word_list.split(' ')

        for w in word_list:

            ln = len(w)

            if w in self.words:
                self.words[w] += 1
            else:
                self.words[w] = 1

            if ln in self.word_lengths:
                self.word_lengths[ln] += 1
            else:
                self.word_lengths[ln] = 1


            if stem(w) in self.stems:
                self.stems[stem(w)] += 1
            else:
                self.stems[stem(w)] = 1

    def add_file(self, filename):
        """reads in txt file to model"""

        f = open(filename, 'r', encoding = 'utf8', errors = 'ignore')

        for line in f:

            if line[-1] == '\n':
                line = line[:-1]

            self.add_string(line)

    def save_model(self):
        """saves dictionaries for model as txt files"""

        f = open(self.name + '_' + 'words', 'w')
        f.write(str(self.words))
        f.close()

        f2 = open(self.name + '_' + 'word_lengths', 'w')
        f2.write(str(self.word_lengths))
        f2.close()

        f3 = open(self.name + '_' + 'stems', 'w')
        f3.write(str(self.stems))
        f3.close()

        f4 = open(self.name + '_' + 'sentence_lengths', 'w')
        f4.write(str(self.sentence_lengths))
        f4.close()

        f5 = open(self.name + '_' + 'punctuation', 'w')
        f5.write(str(self.punctuation))
        f5.close()

    def read_model(self):
        """reads in dictionaries for model"""

        f = open(self.name + '_' + 'words', 'r')
        d_str = f.read()
        f.close()

        self.words = dict(eval(d_str))

        f2 = open(self.name + '_' + 'word_lengths', 'r')
        d_str = f2.read()
        f2.close()

        self.word_lengths = dict(eval(d_str))

        f3 = open(self.name + '_' + 'stems', 'r')
        d_str = f3.read()
        f3.close()

        self.stems = dict(eval(d_str))

        f4 = open(self.name + '_' + 'sentence_lengths', 'r')
        d_str = f4.read()
        f4.close()

        self.sentence_lengths = dict(eval(d_str))

        f5 = open(self.name + '_' + 'punctuation', 'r')
        d_str = f5.read()
        f5.close()

        self.punctuation = dict(eval(d_str))

    def similarity_scores(self, other):
        """gives similarity scores as a list"""

        word_score = compare_dictionaries(other.words, self.words)
        word_length_score = compare_dictionaries(other.word_lengths, self.word_lengths)
        word_stem_score = compare_dictionaries(other.stems, self.stems)
        sentence_length_score = compare_dictionaries(other.sentence_lengths, self.sentence_lengths)
        punctuation_score = compare_dictionaries(other.punctuation, self.punctuation)

        return [word_score, word_length_score, word_stem_score, sentence_length_score, punctuation_score]

    def classify(self, source1, source2):
        """compares self to two other txt models and gives which is closest"""

        scores1 = self.similarity_scores(source1)
        scores2 = self.similarity_scores(source2)

        print('scores for ' + source1.name + ':', scores1)
        print('scores for ' + source2.name + ':', scores2)

        weighted_sum1 = 10*scores1[0] + 1*scores1[1] + 5*scores1[2] + 7*scores1[3] + 1*scores1[4]
        weighted_sum2 = 10*scores2[0] + 1*scores2[1] + 5*scores2[2] + 7*scores2[3] + 1*scores2[4]

        best = max([[weighted_sum1, source1], [weighted_sum2, source2]])

        print(self.name +' is more likely to have come from ' + best[1].name)

                

        
        
