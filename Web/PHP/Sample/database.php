<!DOCTYPE html>
<html>
   <head>
   </head>
   
   <body>

<?php
$dbparams = parse_ini_file("database.ini");

$mysqli = new mysqli($dbparams['dbhost'], $dbparams['dbuser'], $dbparams['dbpass'], $dbparams['dbname']);
if ($mysqli->connect_errno) {
   echo "Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
}
?>
<font color="red">
<?php
echo "Connected to " . $mysqli->host_info . "<br/>";

$mysqli->real_query("SHOW CREATE DATABASE `" . $dbparams['dbname'] . "`");
$res = $mysqli->use_result();

// process all rows
while ($row = $res->fetch_assoc()) {

	// process all columns
	$arr = array_values($row);
	foreach($arr as $key => $val)
		echo $val . "<br/>";
}

?>
</font>

<h2>Available tables:</h2>

<?php
$mysqli->real_query("SHOW TABLES");
$res = $mysqli->use_result();

// process all rows
while ($row = $res->fetch_assoc()) {

   // process all columns
   $arr = array_values($row);
   foreach($arr as $key => $val)
      echo $val . "<br/>";
}
?>

<h2>All data from the mydata table:</h2>

<?php

$mysqli->real_query("SELECT * FROM mydata");
$res = $mysqli->use_result();

// process all rows
while ($row = $res->fetch_assoc()) {

   // process all columns
   $arr = array_values($row);
   foreach($arr as $key => $val)
      echo $val . "|";
   echo "<br/>";
}

?>

<h2>Insert new data:</h2>

<form action="demo_form.asp">
  <textarea rows="10" cols="40"></textarea>
  <input type="submit" value="Submit">
</form>

  </body>

</html>
