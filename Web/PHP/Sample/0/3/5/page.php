<?php

function getRenderValues() {
  $map = Session::current()->getRequestMap();
  
  $dbparams = parse_ini_file("database.ini");
  
  $mysqli = new mysqli($dbparams['dbhost'], $dbparams['dbuser'], $dbparams['dbpass'], $dbparams['dbname']);
  if ($mysqli->connect_errno) {
    echo "Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
  }

  // echo "Connected to " . $mysqli->host_info . "<br/>";
/***********************/  
  $mysqli->real_query("SHOW CREATE DATABASE `" . $dbparams['dbname'] . "`");
  $res = $mysqli->use_result();
 
  // process all rows
  $result = "";
  while ($row = $res->fetch_assoc()) {
  
  	// process all columns
  	$arr = array_values($row);
  	foreach($arr as $key => $val)
  		$result = $val . "<br/>";
  }
  $map['data']['created'] = $result;

/***********************/  
  $mysqli->real_query("SHOW TABLES");
  $res = $mysqli->use_result();
  
  // process all rows
  $result = "";
  while ($row = $res->fetch_assoc()) {
  
    // process all columns
    $arr = array_values($row);
    foreach($arr as $key => $val)
  		$result = $val . "<br/>";
  }
  $map['data']['tables'] = $result;

/************************/
  $mysqli->real_query("SELECT * FROM mydata");
  $res = $mysqli->use_result();
  
  // process all rows - TODO: still separate data from representation - 
  // only return the rows/columns, and create a table component which 
  // uses the data!! No html tags here!
  $result = "<table border=1>";
  while ($row = $res->fetch_assoc()) {
  
    // process all columns
    $result = $result . "<tr>";
    $arr = array_values($row);
    foreach($arr as $key => $val)
      $result = $result . "<td>" . $val . "</td>";
    $result = $result . "</tr>";
  }
  $result = $result . "</table>";
  $map['data']['mydata'] = $result;

  Session::current()->setRequestMap($map);
}

?>
