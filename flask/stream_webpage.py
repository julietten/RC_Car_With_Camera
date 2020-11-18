import os
import datetime
from flask import Flask, render_template, request, redirect
from paramiko import SSHClient

ssh = SSHClient()

app = Flask(__name__)

#default route for the app that shows the set up page
@app.route('/', methods=['GET','POST'])
def setup():
	'''first page of application
	displays the setup html page in the templates folder'''
	return render_template('setup.html')

#route that uses GET and POST http methods for setting up and logging off the user's rpi
@app.route('/submit', methods=['GET','POST'])
def submit():
	'''if the submit button is clicked on the setup page, then the information given is used to run the rpi.py file
	if the logoff button is clicked on the streaming page, then the user ends the ssh connection
	clicks for either button sends the user back to the setup page'''
	if request.method == 'POST' and request.form.get('button1'):  #if the submit button on the setup page is clicked
		user = ""  #stores the rpi username
		host = ""  #stores the rpi hostname
		password = ""  #stores the rpi password
		rpi_py = ""  #stores the path to the rpi.py file on the user's rpi
		if not request.form.get('user') is None:  #checks if the user entered their rpi username
			user = request.form.get('user')  #stores the rpi username if given
		if not request.form.get('host') is None:  #checks if the user entered their rpi hostname
			host = request.form.get('host')  #stores the rpi hostname if given
		if not request.form.get('password') is None:  #checks if the user entered their rpi password
			password = request.form.get('password')  #stores the rpi password if given
		if not request.form.get('rpi_py') is None:  #checks if the user entered their rpi.py path
			rpi_py = request.form.get('rpi_py')  #stores the rpi.py path if given
		ssh = SSHClient()  #stores an SSH connection
		ssh.load_system_host_keys()  #finds the rpi's host keys
		ssh.connect(hostname=host, username=user, password=password)  #connects to the rpi using the given username, hostname, and password
		ssh.exec_command('cd ' + rpi_py + '; python rpi.py')  #runs the rpi.py file through the SSH connection
	if request.method == 'POST' and request.form.get('button2'):  #if the logoff button on the streaming page is clicked
		ssh.exec_command('sudo shutdown -h now')  #runs the shutdown command to disconnect from SSH
	return render_template('setup.html')  #sends the user to the setup html page if either button is clicked

#route that uses the GET and POST http methods and runs the documentation page
@app.route('/streaming', methods=['GET','POST'])
def streaming():
	'''documentation page
	allows user to enter information about the stream including observations
	displays date in the top left'''
	current = datetime.datetime.now()  #stores the current date
	return render_template('streaming.html', date=current.strftime("%m-%d-%Y"))  #runs the documentation page with the date displayed

if __name__ == '__main__':
	app.run() #runs the Flask application, opens '/' route first
