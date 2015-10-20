import android

BT_DEVICE_ID = '98:D3:32:70:55:D9'

droid = android.Android()
"""The first parameter is the service UUID for SerialPortServiceClass.
The second one is the address of your bluetooth module.
If the second one is ommited, Android shows you a selection at program start.
When this function succeeds the little led on the bluetooth module should stop blinking.
"""
droid.toggleBluetoothState(True)
if droid.checkBluetoothState().result == True:
	print 'Bluetooth is on'
else:
	print "could not turn on bluetooth"

print "Phone info:" + str(droid.bluetoothGetLocalName())
print "Computer info: " + str(droid.bluetoothGetRemoteDeviceName(BT_DEVICE_ID))
	
droid.bluetoothConnect('00001101-0000-1000-8000-00805F9B34FB', BT_DEVICE_ID)

droid.webViewShow('file:///sdcard/sl4a/scripts/rs-red.html')

while True:
	result = droid.eventWaitFor('onClick').result
	data = result['data']
	#droid.makeToast(data)
	if data == "0":
		break
		
	
	droid.bluetoothWrite(data + "\n")
	
	droid.makeToast(data)
	#droid.bluetoothWrite(' ')  # send a space to your arduino to signal it to read a value from the sensor.
	#sensor_data = droid.bluetoothReadLine().result  # read the line with the sensor value from arduino.
	#droid.eventClearBuffer()  # workaround for a bug in SL4A r4.
	#droid.eventPost('display_data', result)  # send an event with the sensor data back to the html page.

droid.bluetoothStop()