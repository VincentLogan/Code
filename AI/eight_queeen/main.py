import numpy as np           # 提供维度数组与矩阵运算
import copy                  # 从copy模块导入深度拷贝方法
from board import Chessboard

# 基于棋盘类，设计搜索策略
class Game:
    def __init__(self, show = True):
        """
        初始化游戏状态.
        """

        self.chessBoard = Chessboard(show)
        self.solves = []
        self.gameInit()

    # 重置游戏
    def gameInit(self, show = True):
        """
        重置棋盘.
        """

        self.Queen_setRow = [-1] * 8
        self.chessBoard.boardInit(False)

    ##############################################################################
    ####                请在以下区域中作答(可自由添加自定义函数)                 ####
    ####              输出：self.solves = 八皇后所有序列解的list                ####
    ####             如:[[0,6,4,7,1,3,5,2],]代表八皇后的一个解为                ####
    ####           (0,0),(1,6),(2,4),(3,7),(4,1),(5,3),(6,5),(7,2)            ####
    ##############################################################################
    #                                                                            #

    def run(self, row=0):
        self.search(self.chessBoard, row)

    def search(self, board, row):
        if row < 7:
            for i in range(8):
                if board.setLegal(row, i):
                    self.Queen_setRow[row] = i
                    new_board = copy.deepcopy(board)
                    new_board.setQueen(row, i)
                    self.search(new_board, row+1)
                    self.Queen_setRow[row] = -1
        else:
            for i in range(8):
                if board.setLegal(row, i):
                    self.Queen_setRow[row] = i
                    self.solves.append(self.Queen_setRow.copy())
                    self.Queen_setRow[row] = -1


    def showResults(self, result):
        """
        结果展示.
        """

        self.chessBoard.boardInit(False)
        for i, item in enumerate(result):
            if item >= 0:
                self.chessBoard.setQueen(i, item, False)

        self.chessBoard.printChessboard(False)

    def get_results(self):
        """
        输出结果(请勿修改此函数).
        return: 八皇后的序列解的list.
        """

        self.run()
        return self.solves

game = Game()
solutions = game.get_results()
print('There are {} results.'.format(len(solutions)))
game.showResults(solutions[0])