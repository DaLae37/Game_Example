import tkinter as tk
import random
import math

class GameObject(object):
    def __init__(self, canvas, item):
        self.canvas = canvas
        self.item = item

    def get_position(self):
        return self.canvas.coords(self.item)

    def move(self, x, y):
        self.canvas.move(self.item, x, y)

    def delete(self):
        self.canvas.delete(self.item)


class Ball(GameObject):
    def __init__(self, canvas, x, y):
        self.radius = 10.5
        self.direction = [1 / math.sqrt(2), -1 / math.sqrt(2)] #벡터의 크기를 0으로
        self.speed = 10
        self.fileName = tk.PhotoImage(file = 'ball2.png')
        self.item = canvas.create_image(x, y, anchor=tk.CENTER, image=self.fileName)
        super(Ball, self).__init__(canvas, self.item)

    def update(self):
        coords = self.get_position()
        width = self.canvas.winfo_width()
        if coords[0] <= 0 or coords[2] >= width:
            self.direction[0] *= -1
        if coords[1] <= 0:
            self.direction[1] *= -1
        x = self.direction[0] * self.speed
        y = self.direction[1] * self.speed
        self.move(x, y)

    def is_point_in_rect(self, rect, point) :
        left = rect[0]
        top = rect[1]
        right = rect[2]
        bottom = rect[3]

        x = point[0]
        y = point[1]

        return (x >= left and x <= right and y <= top and y >= bottom)
    
    def collide(self, game_objects):
        coords = self.get_position()
        circle_x = (coords[0] + coords[2]) * 0.5
        circle_y = (coords[1] + coords[3]) * 0.5
        if len(game_objects) > 1:
            self.direction[1] *= -1
        elif len(game_objects) == 1:
            game_object = game_objects[0]
            if isinstance(game_objects[0], Paddle) :
                if circle_x > (game_objects[0].get_position()[0] + game_objects[0].get_position()[2]) * 0.5:
                    self.direction[0] = 1 / math.sqrt(2)
                else :
                    self.direction[0] = -1 / math.sqrt(2)
                self.direction[1] *= -1
            else :
                brick_coords = game_object.get_position()
                x1 = brick_coords[0] - 10.5
                y1 = brick_coords[3] + 10.5
                x2 = brick_coords[2] + 10.5
                y2 = brick_coords[1] - 10.5
                rr = self.radius * self.radius
                direction_x = self.direction[0]
                direction_y = self.direction[1]

                col_x1 = 0
                col_y1 = 0
                col_x2 = 0
                col_y2 = 0
                set_1 = False
                set_2 = False

                #원의 방정식 전개
                x = -(2 * circle_x)
                c1 = -rr + (y1 * y1) - (2 * y1 * circle_y) + (circle_y * circle_y) + (circle_x * circle_x)
                y = -(2 * circle_y)    
                c2 = -rr + (x1 * x1) - (2 * x1 * circle_x) + (circle_x * circle_x) + (circle_y * circle_y)
            
                if (circle_x * circle_x) - c1 >= 0 : #판별식
                    result_x_positive = circle_x + math.sqrt((circle_x * circle_x) - c1)
                    result_x_negative = circle_x - math.sqrt((circle_x * circle_x) - c1)
                    
                    if self.is_point_in_rect([x1,y1,x2,y2], [result_x_positive, y1]):
                        col_x1 = result_x_positive
                        col_y1 = y1
                        set_1 = True
                    
                    if self.is_point_in_rect([x1,y1,x2,y2], [result_x_negative, y1]):
                        col_x1 = result_x_negative
                        col_y1 = y1
                        set_1 = True
                
                if (circle_y * circle_y) - c2 >= 0 : #판별식
                    result_y_positive = - circle_y - math.sqrt((circle_y * circle_y) - c2)
                    result_y_negative = - circle_y - math.sqrt((circle_y * circle_y) - c2)

                    if self.is_point_in_rect([x1,y1,x2,y2], [x1, result_y_positive]):
                        if set_1 :
                            col_x2 = x1
                            col_y2 = result_y_positive
                            set_2 = True
                        else :
                            col_x1 = x1
                            col_y1 = result_y_positive
                    
                    if self.is_point_in_rect([x1,y1,x2,y2], [x1, result_y_negative]):
                        if set_1 :
                            col_x2 = x1
                            col_y2 = result_y_negative
                            set_2 = True
                        else :
                            col_x1 = x1
                            col_y1 = result_y_negative
    
                if set_1 == False or set_2 == False :
                    x = -(2 * circle_x)
                    c1 = -rr + (y2 * y2) - (2 * y2 * circle_y) + (circle_y * circle_y) + (circle_x * circle_x)
                    y = -(2 * circle_y)    
                    c2 = -rr + (x2 * x2) - (2 * x2 * circle_x) + (circle_x * circle_x) + (circle_y * circle_y)
            
                    if (circle_x * circle_x) - c1 >= 0 : #판별식
                        result_x_positive = circle_x + math.sqrt((circle_x * circle_x) - c1)
                        result_x_negative = circle_x - math.sqrt((circle_x * circle_x) - c1)
                
                        if self.is_point_in_rect([x1,y1,x2,y2], [result_x_positive, y2]):
                            if set_1 :
                                col_x2 = result_x_positive
                                col_y2 = y2
                                set_2 = True
                            else :
                                col_x1 = result_x_positive
                                col_y1 = y2
                        
                        if self.is_point_in_rect([x1,y1,x2,y2], [result_x_negative, y2]):
                            if set_1 :
                                col_x2 = result_x_negative
                                col_y2 = y2
                                set_2 = True
                            else :
                                col_x1 = result_x_negative
                                col_y1 = y2
                
                    if (circle_y * circle_y) - c2 >= 0 : #판별식
                        result_y_positive = - circle_y - math.sqrt((circle_y * circle_y) - c2)
                        result_y_negative = - circle_y - math.sqrt((circle_y * circle_y) - c2)
    
                        if self.is_point_in_rect([x1,y1,x2,y2], [x2, result_y_positive]):
                            if set_1 :
                                col_x2 = x2
                                col_y2 = result_y_positive
                            else :
                                col_x1 = x2
                                col_y1 = result_y_positive
                    
                        if self.is_point_in_rect([x1,y1,x2,y2], [x2, result_y_negative]):
                            if set_1 :
                                col_x2 = x2
                                col_y2 = result_y_negative
                            else :
                                col_x1 = x2
                                col_y1 = result_y_negative

                #법선 벡터 두 개 구하기, A(x,y)일 때 법선 벡터는 B(y, -x)
                vector_x1 = col_y1 - col_y2
                vector_y1 = -(col_x1 - col_x2)

                vector_x2 = col_y2 - col_y1
                vector_y2 = -(col_x2 - col_x1)
                
                if abs(math.atan2(vector_y1, vector_x1) - math.atan2(direction_y, direction_x)) * 180 / math.pi < 90 :
                    # 진행 방향과 법선벡터의 사이각이 90도 아래일경우 바깥족으로 향하는 법선 벡터
                    degree = abs(math.atan2(vector_y1, vector_x1) - math.atan2(direction_y, direction_x)) * 180 / math.pi
                    
                else :
                    degree = abs(math.atan2(direction_y, direction_x) - math.atan2(vector_y1, vector_x1)) * 180 / math.pi
                    
                vector_x = (math.cos(degree) * direction_x - math.sin(degree) * direction_y) * 2 #회전행렬
                vector_y = (math.sin(degree) * direction_x + math.cos(degree) * direction_y) * 2
                vector_size = math.sqrt(vector_x ** 2 + vector_y ** 2) #정규화
    
                if vector_size == 0 :
                    self.direction[0] = 1 / math.sqrt(2)
                    self.direction[1] = -1 / math.sqrt(2)
                else :
                    self.direction[0] = vector_x / vector_size
                    self.direction[1] = vector_y / vector_size

        for game_object in game_objects:
            if isinstance(game_object, Brick):
                game_object.hit()

    def get_position(self):
        x, y = self.canvas.coords(self.item)
        return (x - self.radius, y - self.radius, x + self.radius, y + self.radius)
    
class Paddle(GameObject):
    def __init__(self, canvas, x, y):
        self.width = 80
        self.height = 10
        self.ball = None
        item = canvas.create_rectangle(x - self.width / 2,
                                       y - self.height / 2,
                                       x + self.width / 2,
                                       y + self.height / 2,
                                       fill='blue')
        super(Paddle, self).__init__(canvas, item)

    def set_ball(self, ball):
        self.ball = ball

    def move(self, offset):
        coords = self.get_position()
        width = self.canvas.winfo_width()
        if coords[0] + offset >= 0 and coords[2] + offset <= width:
            super(Paddle, self).move(offset, 0)
            if self.ball is not None:
                self.ball.move(offset, 0)


class Brick(GameObject):
    COLORS = {1: '#999999', 2: '#888888', 3: '#777777', 4 : '#666666', 5 : '#555555', 6 : '#444444', 7 : '#333333', 8 : '#222222', 9 : '#111111', 10 : '#000000'}

    def __init__(self, canvas, x, y, hits):
        self.width = 75
        self.height = 20
        self.hits = hits
        color = Brick.COLORS[hits]
        item = canvas.create_rectangle(x - self.width / 2,
                                       y - self.height / 2,
                                       x + self.width / 2,
                                       y + self.height / 2,
                                       fill=color, tags='brick')
        super(Brick, self).__init__(canvas, item)

    def hit(self):
        self.hits -= 1
        if self.hits == 0:
            self.delete()
        else:
            self.canvas.itemconfig(self.item,
                                   fill=Brick.COLORS[self.hits])


class Game(tk.Frame):
    def __init__(self, master):
        super(Game, self).__init__(master)
        self.level = 0
        self.lives = 3
        self.width = 610
        self.height = 400
        self.canvas = tk.Canvas(self, bg='#aaaaff', width=self.width, height=self.height,)
        self.canvas.pack()
        self.pack()

        self.items = {}
        self.ball = None
        self.paddle = Paddle(self.canvas, self.width/2, 326)
        self.items[self.paddle.item] = self.paddle

        self.hud = None
        self.canvas.focus_set()
        self.setup_level()
        self.canvas.bind('<Left>', lambda _: self.paddle.move(-10))
        self.canvas.bind('<Right>', lambda _: self.paddle.move(10))

    def setup_game(self):
        self.canvas.unbind('<space>')
        self.add_ball()
        self.update_lives_text()
        self.text = self.draw_text(300, 200,'Press Space to start')
        self.canvas.bind('<space>', lambda _: self.start_game())

    def setup_level(self) :
        if self.level != 0 :
            self.canvas.delete(self.text)
        self.level+=1
        firstLayer = random.randint(1, self.level)
        secondLayer = random.randint(1, self.level + 1)
        thirdLayer = random.randint(1,self.level + 2)
        for x in range(5, self.width - 5, 75):
            isCreateFirst = random.randint(0,10)
            isCreateSecond = random.randint(0,10)
            isCreateThird = random.randint(0,10)
            if isCreateThird < 4 + self.level // 2:
                self.add_brick(x + 37.5, 50, thirdLayer)
            if isCreateSecond < 5 + self.level // 2: 
                self.add_brick(x + 37.5, 70, secondLayer)
            if isCreateFirst< 6 + self.level // 2 :
                self.add_brick(x + 37.5, 90, firstLayer)
        self.setup_game()
        
    def add_ball(self):
        if self.ball is not None:
            self.ball.delete()
        paddle_coords = self.paddle.get_position()
        x = (paddle_coords[0] + paddle_coords[2]) * 0.5
        self.ball = Ball(self.canvas, x, 310)
        self.paddle.set_ball(self.ball)

    def add_brick(self, x, y, hits):
        brick = Brick(self.canvas, x, y, hits)
        self.items[brick.item] = brick

    def draw_text(self, x, y, text, size='40'):
        font = ('Helvetica', size)
        return self.canvas.create_text(x, y, text=text, font=font)

    def update_lives_text(self):
        text = 'Lives: %s Level : %d' % (self.lives, self.level)
        if self.hud is None:
            self.hud = self.draw_text(80, 20, text, 15)
        else:
            self.canvas.itemconfig(self.hud, text=text)

    def start_game(self):
        self.canvas.unbind('<space>')
        self.canvas.delete(self.text)
        self.paddle.ball = None
        self.game_loop()

    def game_loop(self):
        self.check_collisions()
        num_bricks = len(self.canvas.find_withtag('brick'))
        if num_bricks == 0:
            self.ball.speed = None
            if self.level == 7 :
                self.draw_text(300, 200, 'You Win!')
            else :
                self.text = self.draw_text(300, 200, 'Level Clear')
                self.canvas.bind('<space>', lambda _: self.setup_level())
        elif self.ball.get_position()[3] >= self.height: 
            self.ball.speed = None
            self.lives -= 1
            if self.lives < 0:
                self.draw_text(300, 200, 'Game Over')
            else:
                self.after(1000, self.setup_game)
        else:
            self.ball.update()
            self.after(50, self.game_loop)

    def check_collisions(self):
        ball_coords = self.ball.get_position()
        items = self.canvas.find_overlapping(*ball_coords)
        objects = [self.items[x] for x in items if x in self.items]
        self.ball.collide(objects)


if __name__ == '__main__':
    root = tk.Tk()
    root.title('Hello, Pong!')
    game = Game(root)
    game.mainloop()
