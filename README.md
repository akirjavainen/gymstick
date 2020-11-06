# Control Gymstick WalkingPad 433.92MHz treadmills
Since I only have one of these products, I'm not able to tell if the remotes have unique IDs or if they all send identical commands. My guess would be that the first 21 bits of the 25 bit command is the (unique) remote ID.

# How to use
Capture the binary commands from your remote with RemoteCapture.ino and copy paste the 25 bit commands to the #defines. Then you can control your treadmill with sendGymstickCommand(GYMSTICK_START_STOP), sendGymstickCommand(GYMSTICK_PLUS) etc.

# What exactly is the point of all this?
I wanted to be able to control my treadmill over Alexa. I created applets to IFTTT with Alexa voice commands, sending Webhooks requests to one of my Raspberry Pis. Additionally, I set up Alexa routines to remove the need for the additional "trigger" keyword. Now I can simply shout (for example) "Alexa, treadmill" or "Alexa, walk faster" to control the WalkingPad. No need for a remote or having the mobile at hand.
