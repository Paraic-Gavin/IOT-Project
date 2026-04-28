// To create literal strings stored in flash memory enclose your HTML code between

// F(R"=====( HTML code here )=====");

// If you have 1 reading then you probably have 2 literal strings

// If you have 2 readings then you probably have 3 literal strings etc.

 

String homePage = F(R"=====(<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">

  <title>Smart Classroom Energy Saver</title>

  <!-- Refresh every 2 seconds -->
  <meta http-equiv="refresh" content="2">

  <style>
    body {
      background-color: DodgerBlue;
    }

    .flex-Container {
      display: flex;
      flex-direction: column;
      align-items: center;
    }

    h1 {
      font-size: 40px;
      font-family: Arial;
      color: navy;
      text-align: center;
    }

    p, th, td {
      font-size: 25px;
      font-family: Arial;
      color: navy;
      text-align: center;
    }

    table {
      border-collapse: collapse;
    }

    th, td {
      padding: 10px;
      border-bottom: 1px solid #ddd;
    }
  </style>
</head>

<body>
  <div class="flex-Container">
    <h1>Smart Classroom Energy Saver</h1>
    <p>Live sensor data from ESP32</p>

    <table>
      <tr>
        <th>Sensor</th>
        <th>Value</th>
        <th>Unit</th>
      </tr>

      <tr>
        <td>Temperature</td>
        <td>%TEMP%</td>
        <td>°C</td>
      </tr>

      <tr>
        <td>Humidity</td>
        <td>%HUM%</td>
        <td>%</td>
      </tr>

      <tr>
        <td>PIR Motion</td>
        <td>%PIR%</td>
        <td>Status</td>
      </tr>

    </table>
  </div>
</body>
</html>
)=====");
