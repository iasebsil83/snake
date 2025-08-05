#!/usr/bin/python3

# ---------------- IMPORTATIONS ----------------

#useful
from time import sleep
from random import randint
from math import log

#graphics
from tkinter import *






# ---------------- DEFINITIONS ----------------

#window
WINDOW_WIDTH  = 200
WINDOW_HEIGHT = 200

#directions
UP    = 0
RIGHT = 1
DOWN  = 2
LEFT  = 3

#speed
STEP = 12

#sizes
SNAKE_CHUNK_RADIUS = 6
DOT_RADIUS         = 6

#allowed radius for getting point
ALLOWED_RADIUS = 12






# ---------------- INITIALIZATION ----------------

#dimensions
WINDOW_WIDTH_2  = int(WINDOW_WIDTH/2)
WINDOW_HEIGHT_2 = int(WINDOW_HEIGHT/2)

#initial situation
snake = [[WINDOW_WIDTH_2,WINDOW_HEIGHT_2,RIGHT]]
point =  [WINDOW_WIDTH_2,WINDOW_HEIGHT_2,RIGHT]

#input
key = ['']

#execution control
end = False

#window
win = Tk()
win.title('snake')
d = Canvas(win, width=WINDOW_WIDTH, height=WINDOW_HEIGHT, bg='black')
d.pack()






# ---------------- TOOLS ----------------

#user input
def getkey(k):
	key[0] = k.char






# ---------------- EXECUTION ----------------

#user input
d.bind('<KeyPress>',getkey)

#focus
d.focus_set()

#main loop
starter = False
while not end:

	#input affects direction
	if key[0] == 'z':
		snake[0][2] = UP
	elif key[0] == 'q':
		snake[0][2] = LEFT
	elif key[0] == 's':
		snake[0][2] = DOWN
	elif key[0] == 'd':
		snake[0][2] = RIGHT

	#every dot moves according to its own direction
	for a in range(len(snake)):
		if snake[a][2] == UP:
			snake[a][1] -= STEP
		elif snake[a][2] == RIGHT:
			snake[a][0] += STEP
		elif snake[a][2] == DOWN:
			snake[a][1] += STEP
		elif snake[a][2] == LEFT:
			snake[a][0] -= STEP

		#draw dot
		d.create_rectangle(
			snake[a][0]-SNAKE_CHUNK_RADIUS, snake[a][1]-SNAKE_CHUNK_RADIUS,
			snake[a][0]+SNAKE_CHUNK_RADIUS, snake[a][1]+SNAKE_CHUNK_RADIUS,
			fill='white'
		)

	#death condition
	if len(snake) != 1:
		if abs(snake[1][2]-snake[0][2]) == 2:
			end = True

	#transfer directions from head to tail
	lastIndex = len(snake)-1
	for a in range(len(snake)):
		if a != lastIndex:
			snake[lastIndex-a][2] = snake[lastIndex-a-1][2] #transfert the direction of the point before us but in the reverse order

	#get point => increase length
	if snake[0][0] <= point[0]+ALLOWED_RADIUS+1 and snake[0][0] >= point[0]-ALLOWED_RADIUS and snake[0][1] <= point[1]+ALLOWED_RADIUS+1 and snake[0][1] >= point[1]-ALLOWED_RADIUS:
		point = [
			randint(ALLOWED_RADIUS, WINDOW_WIDTH -ALLOWED_RADIUS), #new dot position
			randint(ALLOWED_RADIUS, WINDOW_HEIGHT-ALLOWED_RADIUS)
		]
		lastChunk = snake[lastIndex]

		#new chunk
		newChunk = [lastChunk[0], lastChunk[1], 9] #invalid direction for the moment, will be updated in next iteration
		snake.append(newChunk)

		'''
		#move forward last element in advance ?
		lastDirection = snake[lastIndex][2] #direction that the last element must follow
		if lastDirection == UP:
			snake[lastIndex][1] += STEP
		elif lastDirection == RIGHT:
			snake[lastIndex][0] -= STEP
		elif lastDirection == DOWN:
			snake[lastIndex][1] -= STEP
		elif lastDirection == LEFT:
			snake[lastIndex][0] += STEP
		'''

	#display dot
	d.create_rectangle(
		point[0]-DOT_RADIUS, point[1]-DOT_RADIUS,
		point[0]+DOT_RADIUS, point[1]+DOT_RADIUS,
		fill='green'
	)

	#out of screen
	if snake[0][0] < 0 or snake[0][0] >= WINDOW_WIDTH or snake[0][1] < 0 or snake[0][1] >= WINDOW_HEIGHT:
		end = True

	#refresh
	d.update()
	d.delete(ALL)

	#temporization
	sleep(2/ (5*log(1+len(snake))) )
print("You died")
