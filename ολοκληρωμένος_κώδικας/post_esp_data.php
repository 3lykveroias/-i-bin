<!--
Ο παρακάτω κώδικας βασίστηκε και παραμετροποιήθηκε στον υπάρχοντα κώδικα του Rui Santos στη 
https://github.com/RuiSantosdotme/Cloud-Weather-Station-ESP32-ESP8266/blob/master/code/esp-post-data.php


  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/cloud-weather-station-esp32-esp8266/
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
-->


<?php

// Όνομα του mysql server στο Π.Σ.Δ.
$servername = "myysql_server_name";

// Όνομα της βάσης δεδομένων
$dbname = "database_name";
// Όνομα χρήστη της βάσης δεδομένων
$username = "user_name";
// Password χρήστη της βάσης δεδομένων
$password = "password";

// Το API key χρησιμοποιείται για λόγους ασφαλείας στη μεταφόρτωση των δεδομένων και έχει ήδη οριστεί στο αρχείο με τον κώδικα για το Esp8266. 
// Αν θέλουμε να το αλλάξουμε, πρέπει πρώτα να αλλαχθεί και στο αρχείο με τον κώδικα για το Esp8266.
$api_key_value = "tPmAT5Ab3j7F9";

$api_key= $sensor = $location = $value1 = $value2 = $value3 = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    if($api_key == $api_key_value) {
        $sensor = test_input($_POST["sensor"]);
        $location = test_input($_POST["location"]);
        $value1 = test_input($_POST["value1"]);
        $value2 = test_input($_POST["value2"]);
        $value3 = test_input($_POST["value3"]);
        
        // Δημιουργία σύνδεσης με τη βάση δεδομένων στο Π.Σ.Δ.
        $conn = new mysqli($servername, $username, $password, $dbname);
        // Έλεγχος σύνδεσης (σε περίπτωση αδυναμία σύνδεσης εμφανίζεται στο μήνυμα "Connection failed: "
        if ($conn->connect_error) {
            die("Connection failed: " . $conn->connect_error);
        } 
        
        $sql = "INSERT INTO SensorData (sensor, location, value1, value2, value3)
        VALUES ('" . $sensor . "', '" . $location . "', '" . $value1 . "', '" . $value2 . "', '" . $value3 . "')";
        
        if ($conn->query($sql) === TRUE) {
            echo "New record created successfully";
        } 
        else {
            echo "Error: " . $sql . "<br>" . $conn->error;
        }
    
        $conn->close();
    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}