

class ChessBoard() :

    def __init__(self, width = 9, height = 9) :
        self.board = [[0 for i in range(width)] for i in range(height)]
