from flask import Flask, render_template, request
import RPi.GPIO as GPIO
from time import sleep
import smbus


addr = 0x55
bus = smbus.SMBus(1)

app = Flask(__name__, template_folder='template')
app.config['DEBUG'] = True

@app.route("/")
def home():
	dist = bus.read_byte(addr)
	print(dist)
	dist = '{0:0.2f}' .format(dist)
	distance = 'Distance: ' + dist
	message = ''
	templateData = {
		'distance' : distance,
		'message' : message
	}
	
	return render_template('index.html', **templateData)

@app.route("/Left", methods=['GET','POST'])
def Left():
	print("Rotate Left")
	dist = bus.read_byte_data(addr, 0x00)
	print(dist)
	dist = '{0:0.2f}' .format(dist)
	distance = 'Distance: ' + dist
	bus.write_byte_data(addr, 0x01, 1)
	message = 'Rotate Left'
	templateData = {
		'distance' : distance,
		'message' : message
	}
	
	return render_template('index.html', **templateData)

@app.route("/Right", methods=['GET','POST'])
def Right():
	print("Rotate Right")
	dist = bus.read_byte_data(addr,0x00)
	print(dist)
	dist = '{0:0.2f}' .format(dist)
	distance = 'Distance: ' + dist
	bus.write_byte_data(addr, 0x01, 2)
	message = 'Rotate Right'
	templateData = {
		'distance' : distance,
		'message' : message
	}
	
	return render_template('index.html', **templateData)

if __name__ == "__main__":
	app.run(host='192.168.1.14', port=80)