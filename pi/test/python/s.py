import socket

MESSAGE = "1234567890"



if __name__ == '__main__':

	ssock = socket.socket(socket.AF_INET,socket.SOCK_DGRAM)

	sadd = ('',3301)

	ssock.bind(sadd)
	
	
	try:

		while 1:

			data,addr = ssock.recvfrom(128)

			print(data.decode())
				
			if(data.decode() == MESSAGE):

				print (MESSAGE)

				ssock.sendto(MESSAGE.encode(),addr)	

	except KeyboardInterrupt:

		print ("Terminate")

		ssock.close()
