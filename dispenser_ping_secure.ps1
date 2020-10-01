# This block gets rid of the initial serial port error
$port.Open()
Start-Sleep -m 1000
$port.Close()
Start-Sleep -m 1000

$port.ReadTimeout = 10000
# Open serial port
$port.Open()
# Ping arduino to get a response
$port.WriteLine("Check")
# Get either washed or unwashed
$answer = $port.ReadLine()
# Remove annoying ending space
$answer = $answer.Substring(0,$answer.Length-1)
# Close serial port
$port.Close()

# Output whether the user is a good person or not
If ($answer -ne "Washed") {
  # If user cares not for their peers
  $output = 'Please sanitize hands before using the computer.'
  # Creating object os WScript
  $wshell = New-Object -ComObject Wscript.Shell -ErrorAction Stop
  # Invoking the POP method using object
  $wshell.Popup($output,5,'Dispenser Warning',$default)
  # Force logout
  shutdown.exe -l
}