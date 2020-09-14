# For optional arguments
$default = [Type]::Missing

# Create object for arduino serial port
$port= new-Object System.IO.Ports.SerialPort COM3,9600,None,8,one
$port.ReadTimeout = 50
# Open serial port
$port.Open()
$port.DiscardInBuffer()
# Ping arduino to get a response
$port.WriteLine("Check")
# Get either washed or unwashed
$answer = $port.ReadLine()
# Remove annoying ending space
$answer = $answer.Substring(0,$answer.Length-1)
# Close serial port
$port.Close()

# Output whether the user is a good person or not
If ($answer -eq "Unwashed") {
  # If user cares not for their peers
  $output = 'Please sanitize hands before using the computer.'
  # Creating object os WScript
  $wshell = New-Object -ComObject Wscript.Shell -ErrorAction Stop
  # Invoking the POP method using object
  $wshell.Popup($output,$default,$default,1)
  # Force logout
  shutdown.exe -l
}
