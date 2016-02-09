# RB5X WiiMote control script
# Receive commands from a WiiMote and convert them into commands to send to the Arduino, which will in turn send them along to Sabertooth controller

import cwiid, time, serial, subprocess

button_delay = 0.1 # used to debounce the buttons


# This code attempts to connect to your Wiimote and if it fails the program quits

# Open the serial port to the Arduino
port=serial.Serial("/dev/ttyACM0", baudrate=9600, timeout=3.0)

# First, pair a WiiMote
print "Please connect a WiiMote by pressing buttons 1 + 2 at the same time"
try:
  wii=cwiid.Wiimote()
except RuntimeError:
  print "Cannot connect to your Wiimote. Run again and make sure you are holding buttons 1 + 2!"
  quit()

print 'Wiimote connection established!\n'
print 'Go ahead and press some buttons\n'
print 'Press PLUS and MINUS together to disconnect and quit.\n'

time.sleep(3)

wii.rpt_mode = cwiid.RPT_BTN


while True:

  buttons = wii.state['buttons']

  # Detects whether + and - are held down and if they are it quits the program
  if (buttons - cwiid.BTN_PLUS - cwiid.BTN_MINUS == 0):
    print '\nClosing connection ...'
    # NOTE: This is how you RUMBLE the Wiimote
    wii.rumble = 1
    time.sleep(1)
    wii.rumble = 0
    exit(wii)

  # The following code detects whether any of the Wiimotes buttons have been pressed and then passes the appropriate command to the Arduino
  # The codes that are sent to the Arduino are:
  # 0 = STOP
  # 1 = FWD
  # 2 = REVERSE
  # 3 = TURN CLOCKWISE
  # 4 = TURN COUNTERCLOCKWISE
  # 1 = custom action (play a video)
  if (buttons & cwiid.BTN_1):
	subprocess.call(['cvlc', '/nfltheme.mp3'])
	time.sleep(button_delay)
  # LEFT = CCW
  if (buttons & cwiid.BTN_LEFT):
    #print 'Left pressed'
    port.write("4")
    time.sleep(button_delay)
   # RIGHT = CW
  if(buttons & cwiid.BTN_RIGHT):
    #print 'Right pressed'
    port.write("3")
    time.sleep(button_delay)
   # UP = FWD
  if (buttons & cwiid.BTN_UP):
    #print 'Up pressed'
    port.write("1")
    time.sleep(button_delay)
   # DN = REV
  if (buttons & cwiid.BTN_DOWN):
    #print 'Down pressed'
    port.write("2")
    time.sleep(button_delay)
   # B = STOP
  if (buttons & cwiid.BTN_B):
    #print 'Button B pressed'
    port.write("0")
    time.sleep(button_delay)


