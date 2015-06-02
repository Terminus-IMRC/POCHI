#!/usr/bin/env python3

import sys

def error(str):
	print("error:", str, file = sys.stderr)

def str_to_entry(str, prev_timing):
	tokens = str.split()

	l = len(tokens)
	if l == 0:
		return None
	if l != 3:
		error("token length is not 3 but %d" % l)
		exit(1)

	try:
		timing = int(tokens[0])
	except ValueError:
		error("could not convert the timing into integer: %s" % tokens[0])
		exit(1)
	if tokens[0][0] == '+':
		timing += prev_timing

	try:
		direction = int(tokens[1])
	except ValueError:
		error("could not convert the direction into integer: %s" % tokens[1])
		exit(1)
	if direction != 0 and direction != 1:
		error("direction out-of-range: %d" % direction)

	try:
		degrees = int(tokens[2])
	except ValueError:
		error("could not convert the degrees into integer: %s" % tokens[1])
		exit(1)
	if degrees < -90 or degrees > 90:
		error("degrees out-of-range: %d" % degrees)
	
	return (timing, direction, degrees)

def main():
	prev_timing = 0
	ts = []
	while True:
		try:
			s = input()
		except EOFError:
			break

		t = str_to_entry(s, prev_timing)
		if t == None:
			continue
		ts.append(t)
		prev_timing = t[0]

	ts.sort()
	print("{")
	for i in range(len(ts)):
		t = ts[i]
		print("\t/* %d */ {%d, %d, %d}," % (i, t[0], t[1], t[2]))
	print("}")

if __name__ == '__main__':
	main()
