import RPi.GPIO as GPIO
from gpiozero import Button
import time
import os

#ecse398-group1

#ssh
#python rpi.py
#To run VLC from the command line: $ vlc source. Replace source with path to the file to be played, URL, or other data source. 

# enable camera in rasp pi desktop envir:
# Preferences -> Raspberry Pi Configuration window -> Interfaces -> Camera

# install vlc on rpi: sudo apt-get install VLC
# also install vlc on desktop***
# for vlc on desktop: Media tab -> Open Network Stream -> type url: http://[insert rpi ip address]:8160

def startCameraStream(): #funciton that starts the camera stream
	#os commands to update the camera -- may not be needed
	os.system("sudo apt update") 
	os.system("sudo apt full-upgrade")

	numVid = "" #stores the video number for naming purposes - to prevent overwriting
	num = 0 #stores the last video number saved before an empty spot
	startVid = "" #name of video
	savedVids = os.listdir("/home/pi/videos") #stores all currently saved items on the rpi desktop
	for item in savedVids: #loops through every item on the rpi desktop
		name = item + "" #stores name of current item name being read
		if item[:6] == "video": #checks that item is a video
			numVid = item[6:-5] #stores the number value of the video
			if numVid == "" and 1 > num: #if there isn't a video saved
				num = 1 #stores number of video as first
			elif Int(numVid) + 1 > num: #if numVid does have a value
				num = Int(numVid) + 1 #stores numVids value incremented
	startVid = "raspivid -o /home/pi/videos/video" + str(num) + ".h264" #command to save video with a name that doesn't overwrite others
	os.system(startVid) #runs command to save video
	setupStream = "raspivid -o - -t 0 -hf -w 800 -h 400 -fps 24 |cvlc -vvv stream:///dev/stdin --sout '#standard{access=http,mux=ts,dst=:8160}' :demux=h264" #command to start camera stream
	os.system(setupStream) #runs command to start camera stream

if __name__ == "__main__":
	startCameraStream() #starts the camera stream
