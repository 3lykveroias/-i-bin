<!--
Ο παρακάτω κώδικας βασίστηκε και παραμετροποιήθηκε στον υπάρχοντα κώδικα του Rui Santos στη 
διεύθυνση https://github.com/RuiSantosdotme/Cloud-Weather-Station-ESP32-ESP8266/blob/master/code/esp-weather-station.php
-->


<!DOCTYPE html>
<html><body>
<?php

// Όνομα του mysql server στο Π.Σ.Δ.
$servername = "myysql_server_name";

// Όνομα της βάσης δεδομένων
$dbname = "database_name";
// Όνομα χρήστη της βάσης δεδομένων
$username = "user_name";
// Password χρήστη της βάσης δεδομένων
$password = "password";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$sql = "SELECT id, sensor, location, value1, value2, value3, reading_time FROM SensorData ORDER BY id DESC";

echo '<table cellspacing="5" cellpadding="5">
	  <tr>
		<center>(i) bin - Καταγραφή συμβάντων</center>
	  </tr>
	  <tr> 
        <td>ID</td> 
        <td>Στάθμη κάδου</td> 
        <td>Ένδειξη καπνού</td> 
        <td>Ώρα καταγραφής</td> 
      </tr>';
 
if ($result = $conn->query($sql)) {
    while ($row = $result->fetch_assoc()) {
        $row_id = $row["id"];
        $row_value1 = $row["value1"];
        $row_value2 = $row["value2"];  
        $row_reading_time = $row["reading_time"];
      
        echo '<tr> 
                <td>' . $row_id . '</td>  
                <td>' . $row_value1 . '</td> 
                <td>' . $row_value2 . '</td> 
                <td>' . $row_reading_time . '</td> 
              </tr>';
    }
    $result->free();
}

$conn->close();
?> 
</table>
</body>
</html>