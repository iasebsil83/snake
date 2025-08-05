#!/usr/bin/python3

# ---------------- IMPORTATIONS ----------------

#system
import sys, os

#terminal
import click

#multithreading
from threading import Thread

#useful
from time   import sleep
from random import randint
from math   import log






# ---------------- DEFINITIONS ----------------

#window
WINDOW_WIDTH  = 40
WINDOW_HEIGHT = 20

#directions
UP    = 0
RIGHT = 1
DOWN  = 2
LEFT  = 3

#speed
STEP = 1

#allowed radius for getting point
ALLOWED_RADIUS = 1






# ---------------- INITIALIZATION ----------------

#dimensions
WINDOW_WIDTH_2  = int(WINDOW_WIDTH/2)
WINDOW_HEIGHT_2 = int(WINDOW_HEIGHT/2)

#initial situation
snake = [[WINDOW_WIDTH_2,WINDOW_HEIGHT_2,RIGHT]]
point =  [WINDOW_WIDTH_2,WINDOW_HEIGHT_2]

#input
key = ['']

#execution control
end = [False]






# ---------------- TOOLS ----------------

#user input
def inputLoop():
	while not end[0]:
		try:
			key[0] = click.getchar()
		except KeyboardInterrupt:
			end[0] = True



#display
def clear():
	os.system("clear")

def printFrame():
	upLine     = "#" * WINDOW_WIDTH + "\r\n"
	middleLine = "#" + " " * (WINDOW_WIDTH-2) + "#\r\n"
	sys.stdout.write(upLine + middleLine * (WINDOW_HEIGHT-2)  + upLine)

def draw(x, y, c):
	sys.stdout.write("\x1b[" + str(y+1) + ";" + str(x+1) + "H")
	sys.stdout.write(c)






# ---------------- EXECUTION ----------------

#input thread
inputThread = Thread(target=inputLoop)
inputThread.start()

#main loop
starter = False
while not end[0]:

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
		draw(snake[a][0], snake[a][1], 'o')

	#death condition
	if len(snake) != 1:
		if abs(snake[1][2]-snake[0][2]) == 2:
			end[0] = True

	#transfer directions from head to tail
	lastIndex = len(snake)-1
	for a in range(len(snake)):
		if a != lastIndex:
			snake[lastIndex-a][2] = snake[lastIndex-a-1][2] #transfert the direction of the point before us but in the reverse order

	#get point => increase length
	if snake[0][0] <= point[0]+ALLOWED_RADIUS+1 and snake[0][0] >= point[0]-ALLOWED_RADIUS and snake[0][1] <= point[1]+ALLOWED_RADIUS+1 and snake[0][1] >= point[1]-ALLOWED_RADIUS:
		point = [
			randint(ALLOWED_RADIUS, WINDOW_WIDTH -ALLOWED_RADIUS), #new position
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
	draw(point[0], point[1], '@')
	sys.stdout.flush()

	#out of screen
	if snake[0][0] < 0 or snake[0][0] >= WINDOW_WIDTH or snake[0][1] < 0 or snake[0][1] >= WINDOW_HEIGHT:
		end[0] = True

	#temporization
	sleep(2/ (5*log(1+len(snake))) )

	#clean
	clear()
	printFrame()
print("You died")
