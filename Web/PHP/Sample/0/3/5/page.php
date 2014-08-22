<?php

function getRenderValues() {
/*
  $dbparams = parse_ini_file("database.ini");
  
  $mysqli = new mysqli($dbparams['dbhost'], $dbparams['dbuser'], $dbparams['dbpass'], $dbparams['dbname']);
  if ($mysqli->connect_errno) {
    echo "Failed to connect to MySQL: (" . $mysqli->connect_errno . ") " . $mysqli->connect_error;
  }

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
  */

  
  $map = Session::current()->getRequestMap();
  $map['data']['lorem'] = "Some new data !!!!";
  Session::current()->setRequestMap($map);
}

?>
