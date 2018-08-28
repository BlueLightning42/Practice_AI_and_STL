from random import choice

# Laughably simple ML
# its perfect state/what its working towards is a state machine with every state mapped out

def checkAllEqual(iterator):
    iterator = iter(iterator)
    try:
        first = next(iterator)
    except StopIteration:
        return True
    return all(first == rest for rest in iterator)

# import replit (clear not working)

            # 0 1 2 
            # 3 4 5 
            # 6 7 8
class board:
    def __init__(self, p1, p2, d):
        self.d = d
        self.b = [0 for _ in range(9)]
        self.p1 = p1
        self.p2 = p2
        self.state = 1 #player 1
        if d:
            print("\n0|1|2\n------\n3|4|5\n------\n6|7|8\n\nFirst Move")
            
        self.play()
    def hashboard(self):
        sm = 0
        for x in self.b:
            sm = sm*10 + (1 if x==0 else (2 if x==1 else 3))
        return sm

    def S(self, a):
        if not self.b[a]: return " "
        if self.b[a] ==1: return "X"
        return "O"

    def show_win(self, winner_is_human, bot):
        print("{}|{}|{}\n------".format(self.S(0),self.S(1),self.S(2)))
        print("{}|{}|{}\n------".format(self.S(3),self.S(4),self.S(5)))
        print("{}|{}|{}\n".format(self.S(6),self.S(7),self.S(8)))
        if winner_is_human:
            print("\n\nYou won! You beat a bot thats already won {} games".format(bot.times_won))
        else:
            print("\n\nThe Bot won but dont feel bad this bot has lost exactly {} times!".format(bot.times_lost))
        # for x,y in bot.game_states.items():
        #     print("state:{} available options: {}".format(x,y))
    
    def showB(self):
        # print("{}|{}|{}\n------\n{}|{}|{}\n------\n{}|{}|{}".format(self.b))
        print("{}|{}|{}\n------".format(self.S(0),self.S(1),self.S(2)))
        print("{}|{}|{}\n------".format(self.S(3),self.S(4),self.S(5)))
        print("{}|{}|{}\n\n".format(self.S(6),self.S(7),self.S(8)))

        if not self.state: 
            print("\nBot's turn")
        else:
            print("\nPlayer's turn")

    def checkwin(self):
        return ((abs(self.b[0]+self.b[1]+self.b[2]) == 3) or
                (abs(self.b[3]+self.b[4]+self.b[5]) == 3) or
                (abs(self.b[6]+self.b[7]+self.b[8]) == 3) or

                (abs(self.b[3]+self.b[0]+self.b[6]) == 3) or
                (abs(self.b[1]+self.b[4]+self.b[7]) == 3) or
                (abs(self.b[2]+self.b[8]+self.b[5]) == 3) or

                (abs(self.b[0]+self.b[4]+self.b[8]) == 3) or
                (abs(self.b[2]+self.b[4]+self.b[6]) == 3))
    def check_tie(self):
        for x in self.b:
            if x == 0: return False
        self.p1.tie()
        self.p2.tie()
        return True
    
    def play(self):
        if self.state:
            mv = self.p1.move(self.hashboard(), self.b)
            if mv == -1: #forfit
                self.check_tie()
                return
            else:
                self.b[mv] = -1
                if self.checkwin():
                    if self.d: return self.show_win(self.p1.is_human, p2)
                    self.p1.reward()
                    self.p2.punish()
                    return

        else:
            mv = self.p2.move(self.hashboard(), self.b)
            if mv == -1: #forfit
                self.check_tie()
                return
            else:
                self.b[mv] = +1
                if self.checkwin():
                    if self.d: return self.show_win(self.p2.is_human, p1)

                    self.p2.reward()
                    self.p1.punish()
                    return
        if(self.d):
            self.showB()
        self.state = not self.state #change state
        self.play()

class TTTbot:
    def __init__(self, human):
        self.is_human = human
        self.game_states = {}
        self.used_game_states = []
        self.times_won = 0
        self.times_lost = 0
        self.times_tied = 0
    
    def move(self, _h, b):
        if self.is_human: 
            while(True):
                i = input("Enter move")
                try:
                    i = int(i)
                except ValueError:
                    print("Not a Number")
                    continue
                
                if i > 8:
                    print("Index too high")
                elif b[i] != 0:
                    print("Cell already taken")
                else:
                    return i

        if _h not in self.game_states or (len(self.game_states[_h]) == 0):
            tmp = []
            for cell in range(len(b)):
                if b[cell] == 0:
                    tmp += [cell,cell,cell,cell,cell]
            
            self.game_states[_h] = tmp # append new state
        # then
        # pick a random index of a valid cell
        if self.game_states[_h] != []:
            pick = choice(self.game_states[_h])
            self.used_game_states.append((_h, pick))
            return pick
        return -1
        
    def reward(self):
        self.times_won += 1

        for state, chosen in self.used_game_states:
            if len(self.game_states[state]) > 100:
                for i in range(9):
                    if i in self.game_states[state]: self.game_states[state].remove(i)
            if checkAllEqual(self.game_states[state]):
                self.game_states[state] = [chosen,chosen,chosen]

            self.game_states[state] += [chosen,chosen,chosen]
            # add more of that choice it was a good one
        state, chosen = self.used_game_states[-1]
        self.game_states[state] = [chosen] # winning move dont pick anything else
        

    def punish(self):
        self.times_lost += 1
        _state, _chosen = self.used_game_states[-1]

        for state, chosen in self.used_game_states:
            if _state in self.game_states[_state]:
                self.game_states[state].remove(chosen)
        
        
        while _state in self.game_states[_state]:   # Baaaad they won with that move
            self.game_states[_state].remove(_state) # never make that choice again stupid
        
        self.used_game_states.clear()

    def tie(self):
        self.times_tied += 1;
        
        self.used_game_states.clear()

p1 = TTTbot(False)

# figured out it works better with random results and if it trains againsed the same bot it stops learning...resetting the bot a few times works well
for _ in range(6): #6 bots
    p2 = TTTbot(False)
    for _2 in range(500): # each bot is trained with it 500 times
        board(p1,p2, False)

#Play player
human = TTTbot(True)
while(True):
	board(p1,human, True)
