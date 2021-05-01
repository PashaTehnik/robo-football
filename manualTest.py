from SerialCommand import Com


s = Com(ARDPath = "/dev/ttyUSB0", portSpeed = 115200)


while (1):
    print("choose motor(0-2)\n")
    mot = int(input())
    print("choose dir(0-1)\n")
    n = int(input())
    print("choose speed(0-28)\n")
    speed = int(input())
    print("cmd = {}".format((mot<<6)|(n<<5)|(speed)))
    s.writeCmd(0|(mot<<6)|(n<<5)|(speed))
    print ("sended: mot = {}, n = {}, speed = {}.".format(mot,n,speed))
