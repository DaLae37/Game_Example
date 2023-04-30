import tkinter as tk
from tkinter import messagebox
import random

class Board() :
    board_type_dict = {0 : "none", 1 : "search", 2 : "flag"}
    def __init__(self, x, y, board_type) :
        self.x = x
        self.y = y
        self.board_type = board_type
        self.is_mine = False
        self.near_mine = 0

    #@Getter
    def get_pos(self) :
        return (x,y)
        
    def get_board_type(self) :
        return self.board_type

    def get_is_mine(self) :
        return self.is_mine

    def get_near_mine(self) :
        return self.near_mine
    
    #@Setter
    def set_board_type(self, board_type) :
        self.board_type = board_type

    def set_is_mine(self, is_mine) :
        self.is_mine = is_mine

    def set_near_mine(self, near_mine) :
        self.near_mine = near_mine

class Game(tk.Frame):
    def __init__(self, master):
        super(Game, self).__init__(master)
        
        menubar = tk.Menu(master)
        filemenu = tk.Menu(menubar, tearoff=0)
        filemenu.add_command(label="9*9", command=self.easy)
        filemenu.add_command(label="16*16", command=self.normal)
        filemenu.add_command(label="16*30", command=self.hard)
        filemenu.add_separator()
        filemenu.add_command(label="Exit", command=master.destroy)
        menubar.add_cascade(label="File", menu=filemenu)
        master.config(menu=menubar)

        self.width = 100
        self.height = 100
        self.square = 50
        self.canvas = tk.Canvas(self, bg="#aaaaff", width=self.width, height=self.height,)
        self.canvas.pack(fill="both", expand=True)
        self.pack(fill="both", expand=True)

        self.canvas.bind("<Button-1>", self.left_button)
        self.canvas.bind("<Button-3>", self.right_button)
      
        self.easy()

    def easy(self) :
        self.column = 9
        self.row = 9
        self.mines = 10
        self.width = self.square * self.column
        self.height = self.square * self.row
        self.canvas.config(width = self.width, height = self.height)
        for i in range(self.row) :
            self.canvas.create_line(0, self.square * i, self.square * self.column, self.square * i)
            for j in range(self.column) :
                self.canvas.create_line(self.square * j, 0, self.square * j, self.square * self.row)
        self.setting_boards()

    def normal(self) :
        self.column = 16
        self.row = 16
        self.mines = 40
        self.width = self.square * self.column
        self.height = self.square * self.row
        self.canvas.config(width = self.width, height = self.height)
        for i in range(self.row) :
            self.canvas.create_line(0, self.square * i, self.square * self.column, self.square * i)
            for j in range(self.column) :
                self.canvas.create_line(self.square * j, 0, self.square * j, self.square * self.row)
        self.setting_boards()
        
    def hard(self) :
        self.column = 30
        self.row = 16
        self.mines = 99
        self.width = self.square * self.column
        self.height = self.square * self.row
        self.canvas.config(width = self.width, height = self.height)
        for i in range(self.row) :
            self.canvas.create_line(0, self.square * i, self.square * self.column, self.square * i)
            for j in range(self.column) :
                self.canvas.create_line(self.square * j, 0, self.square * j, self.square * self.row)
        self.setting_boards()

    def game_over(self, is_win) :
        self.canvas.unbind("<Button-1>")
        self.canvas.unbind("<Button-3>")
        if is_win :
            tk.messagebox.showinfo("Win", "You Win!")
        else :
            tk.messagebox.showinfo("Lose", "You Lose")

    def left_button(self, event) :
        x = event.x // self.square
        y = event.y // self.square
        self.select_region(x, y, 1)

    def right_button(self, event) :
        x = event.x // self.square
        y = event.y // self.square
        self.select_region(x, y, 2)        

    def select_region(self, x, y, select_type) :
        text = 'X'
        if self.boards[y][x].get_board_type() != 2 and select_type == 1 :
            near_mine = self.boards[y][x].get_near_mine()
            text = near_mine
            self.boards[y][x].set_board_type(select_type)
            if self.boards[y][x].get_is_mine() == True :
                text = '◇'
                self.canvas.itemconfig(self.boards_text[y][x], text = text)
                self.game_over(False)
                
            if near_mine == 0 :
                self.recursive_select(x, y)

        elif select_type == 2 :
            if self.boards[y][x].get_board_type() == 2 :
                select_type = 0
                text = ' '
            self.boards[y][x].set_board_type(select_type)
        is_end = self.check_boards()
        if is_end :
            self.game_over(True)

        self.canvas.itemconfig(self.boards_text[y][x], text = text)

    def recursive_select(self, x, y) :
        self.boards[y][x].set_board_type(1)
        near_mine = self.boards[y][x].get_near_mine()
        self.canvas.itemconfig(self.boards_text[y][x], text = near_mine)
        if near_mine == 0 :
            for yy in range(-1, 2) :
                for xx in range(-1, 2) :
                    if x+xx < 0 :
                        continue    
                    if x+xx >= self.column :
                        continue
                    if y+yy < 0 :
                        continue
                    if y+yy >= self.row :
                        continue
                    if yy == 0 and xx == 0 :
                        continue
                    if yy == 0 and xx == 0 :
                        continue

                    if self.boards[y+yy][x+xx].get_board_type() == 0 and self.boards[y+yy][x+xx].get_is_mine() == False :
                        self.recursive_select(x + xx, y + yy)
        
    def detect_region(self, x, y) :
        near_mine = 0
        for yy in range(-1, 2) :
            for xx in range(-1, 2) :
                if x+xx < 0 :
                    continue
                if x+xx >= self.column :
                    continue
                if y+yy < 0 :
                    continue
                if y+yy >= self.row :
                    continue
                if yy == 0 and xx == 0 :
                    continue
                if self.boards[y+yy][x+xx].get_is_mine() == True :
                    near_mine += 1
                    
        self.boards[y][x].set_near_mine(near_mine)

    def check_boards(self) :
        is_end = True
        
        for i in range(self.row) :
            for j in range(self.column) :
                if self.boards[i][j].get_board_type() == 0 :
                    is_end = False
                    return is_end
                
        return is_end
        
    def setting_boards(self) :
        self.boards = [[Board(j,i, 0) for j in range(self.column)] for i in range(self.row)]
        
        while self.mines > 0 :
            x = random.randint(0, self.column -1)
            y = random.randint(0, self.row -1)

            if self.boards[y][x].get_is_mine() == False :
                self.boards[y][x].set_is_mine(True)
                self.mines -= 1

        for i in range(self.row) :
            for j in range(self.column) :
                self.detect_region(i,j)
                
        self.boards_text = [[self.canvas.create_text(j * self.square + (self.square // 2), i * self.square + (self.square // 2), text = ' ') for j in range(self.column)] for i in range(self.row)]
        
if __name__ == '__main__':
    root = tk.Tk()
    root.title('Minesweeper')
    game = Game(root)
