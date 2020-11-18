import time
import os

#setting up camera stream instructions:
#ssh
#download this file onto rpi and in command line run python rpi.py
#enable camera in rpi desktop envir:
#Preferences -> Raspberry Pi Configuration window -> Interfaces -> Camera
#install vlc on rpi: sudo apt-get install VLC
#also install vlc on desktop
#for vlc on desktop: Media tab -> Open Network Stream -> type url: http://[insert rpi ip address]:8160


def startCameraStream():  
	"""function that starts the camera stream
	saves the video on the raspberry pi desktop
	checks that the video being saved doesn't overwrite an existing video"""

	os.system("sudo apt update") 
	os.system("sudo apt full-upgrade")

	numVid = ""  #stores the video number for naming purposes - to prevent overwriting
	num = 0  #stores the last video number saved before an empty spot
	startVid = ""  #stores the name of video
	savedVids = os.listdir("/home/pi/videos")  #stores all currently saved items on the rpi desktop
	for item in savedVids:  #loops through every item on the rpi desktop
		name = item + ""  #stores name of current item name being read
		if item[:6] == "video":  #checks that item is a video
			numVid = item[6:-5]  #stores the number value of the video
			if numVid == "" and 1 > num:  #if there isn't a video saved
				num = 1  #stores number of video as first
			elif Int(numVid) + 1 > num:  #if numVid does have a value
				num = Int(numVid) + 1  #stores numVids value incremented
	startVid = "raspivid -o /home/pi/videos/video" + str(num) + ".h264"  #stores command to save video with a name that doesn't overwrite others
	os.system(startVid)  #runs command to save video
	setupStream = "raspivid -o - -t 0 -hf -vf -w 800 -h 400 -fps 24 |cvlc -vvv stream:///dev/stdin --sout '#standard{access=http,mux=ts,dst=:8160}' :demux=h264"  #command to start camera stream through vlc
	os.system(setupStream)  #runs command to start camera stream


if __name__ == "__main__":
	startCameraStream()  #starts the camera stream and properly saves it on the raspberry pi
